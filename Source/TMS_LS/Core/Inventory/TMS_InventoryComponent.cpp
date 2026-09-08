// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_InventoryComponent.h"

#include <rapidjson/reader.h>

#include "ItemObjects/ItemEquipmentObject.h"
#include "ItemObjects/ItemObject.h"
#include "TMS_LS/Utilities/TMS_DeveloperSettings.h"

//#include "TMS_LS/Core/TMS_DataSettings.h"

// Sets default values for this component's properties
UTMS_InventoryComponent::UTMS_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTMS_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Slots.Init(NewObject<UItemObject>(), ContainerSize);
	OnInventoryInitialized.Broadcast();
}

void UTMS_InventoryComponent::AddItem(TSubclassOf<UItemObject> InItemClass, int32 Amount, bool& OutSuccess,
	EItemRarity Rarity)
{
	OutSuccess = false;
	if (!InItemClass || Amount <=0) return;

	const UItemObject* CDO = InItemClass.GetDefaultObject();

	if (!CDO || CDO->ItemData.ItemID == NAME_None) return;

	const FName ItemID = CDO->ItemData.ItemID;
	int32 Remaining = Amount;

	while (Remaining > 0)
	{
		int32 SlotIndex = -1;
		if (HasNotFullSlotOfItem(ItemID, SlotIndex))
		{
			int32 Overflow = 0;
			if (bool bFilled = TryToFill(SlotIndex, Remaining, Overflow))
			{
				Remaining = 0;
				OutSuccess = true;
				OnInventoryUpdated.Broadcast();
				return;
			}
			else
			{
				Remaining = Overflow;
			}
		}
		else
		{
			UItemObject* NewItem = CreateItemObject(InItemClass, Remaining, Rarity);
			if (NewItem && AddItemToEmptySlot(NewItem))
			{
				Remaining = 0;
				OutSuccess = true;
				OnInventoryUpdated.Broadcast();
				return;
			}
			{
				OutSuccess = false;
				return;
			}
		}
	}
	 OutSuccess = true;
}

void UTMS_InventoryComponent::AddItemAsObject(UItemObject* InItem)
{
	AddItemToEmptySlot(InItem);
	OnInventoryUpdated.Broadcast();
}

void UTMS_InventoryComponent::RemoveItem(TSubclassOf<UItemObject> InItemClass, int32 Amount)
{
	if (!InItemClass || Amount <=0 ) return;

	const UItemObject* CDO = InItemClass.GetDefaultObject();
	if (!CDO || CDO->ItemData.ItemID == NAME_None) return;

	const FName ItemID = CDO->ItemData.ItemID;
	int32 Remaining = Amount;

	while (Remaining > 0)
	{
		int32 SlotIndex = -1;
		if (!FindFirstSlotOfType(SlotIndex, ItemID))
		{
			return;
		}

		UItemObject* Item = Slots[SlotIndex];
		if (!Item || Item->ItemData.ItemID != ItemID) return;

		if (Remaining >= Item->Amount)
		{
			Remaining -=Item->Amount;

			Slots[SlotIndex] = NewObject<UItemObject>(this);
		}
		else
		{
			Item->Amount -= Remaining;
			Remaining = 0;
		}
	}

	OnInventoryUpdated.Broadcast();
}

void UTMS_InventoryComponent::RemoveItemAsObject(UItemObject* InItem)
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i] == InItem)
		{
			Slots[i] = NewObject<UItemObject>();
			OnInventoryUpdated.Broadcast();
		}
	}
}

void UTMS_InventoryComponent::SwapItems(int32 InSlot, int32 OutSlot)
{
	UItemObject* TempItem = Slots[InSlot];
	Slots[InSlot] = Slots[OutSlot];
	Slots[OutSlot] = TempItem;
	OnInventoryUpdated.Broadcast();
}

void UTMS_InventoryComponent::UpdateInventory()
{
	OnInventoryUpdated.Broadcast();
}


void UTMS_InventoryComponent::DEBUG_PrintSlots()
{
}

bool UTMS_InventoryComponent::IsEmpty()
{
	for (const auto& Item : Slots)
	{
		if (Item->ItemData.ItemID != NAME_None) return false;
	}
	return true;
}

bool UTMS_InventoryComponent::SaveInventoryToFile(const FString& FilePath) const
{
	// Создаём корневой JSON-объект
	TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> SlotsArray;

	for (const UItemObject* Item : Slots)
	{
		TSharedPtr<FJsonObject> ItemJson;
		if (Item && Item->ItemData.ItemID != NAME_None)
		{
			ItemJson = Item->SerializeToJson();
		}
		else
		{
			// Пустой слот – сохраняем null или объект с пустым ItemID
			ItemJson = MakeShareable(new FJsonObject);
			ItemJson->SetStringField(UItemObject::JsonKey_ClassName, TEXT(""));
			ItemJson->SetStringField(UItemObject::JsonKey_ItemID, TEXT(""));
			ItemJson->SetNumberField(UItemObject::JsonKey_Amount, 0);
		}
		SlotsArray.Add(MakeShareable(new FJsonValueObject(ItemJson)));
	}

	RootObject->SetArrayField(TEXT("Slots"), SlotsArray);
	RootObject->SetNumberField(TEXT("ContainerSize"), ContainerSize);
	RootObject->SetStringField(TEXT("ContainerName"), ContainerName.ToString());

	// Сериализуем в строку
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (!FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize inventory to JSON"));
		return false;
	}

	// Сохраняем в файл
	if (!FFileHelper::SaveStringToFile(OutputString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save inventory to file: %s"), *FilePath);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Inventory saved to %s"), *FilePath);
	return true;
}

bool UTMS_InventoryComponent::LoadInventoryFromFile(const FString& FilePath)
{
	if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Save file does not exist: %s"), *FilePath);
        return false;
    }

    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load inventory file: %s"), *FilePath);
        return false;
    }

    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON"));
        return false;
    }

    // Читаем размер контейнера и имя (опционально)
    int32 LoadedSize;
    if (RootObject->TryGetNumberField(TEXT("ContainerSize"), LoadedSize))
    {
        ContainerSize = LoadedSize;
    }

    FString ContainerNameString;
    if (RootObject->TryGetStringField(TEXT("ContainerName"), ContainerNameString))
    {
        ContainerName = FText::FromString(ContainerNameString);
    }

    // Читаем слоты
    const TArray<TSharedPtr<FJsonValue>>* SlotsArray;
    if (!RootObject->TryGetArrayField(TEXT("Slots"), SlotsArray))
    {
        UE_LOG(LogTemp, Error, TEXT("Missing 'Slots' array in JSON"));
        return false;
    }

    // Очищаем инвентарь и инициализируем заново
    Slots.Empty();
    Slots.Reserve(ContainerSize);

    for (const TSharedPtr<FJsonValue>& SlotValue : *SlotsArray)
    {
        TSharedPtr<FJsonObject> SlotObject = SlotValue->AsObject();
        if (!SlotObject.IsValid())
        {
            // Если невалидный объект, добавляем пустой слот
            Slots.Add(NewObject<UItemObject>(this));
            continue;
        }

        // Проверяем, есть ли класс
        FString ClassName;
        if (!SlotObject->TryGetStringField(UItemObject::JsonKey_ClassName, ClassName) || ClassName.IsEmpty())
        {
            // Пустой слот
            Slots.Add(NewObject<UItemObject>(this));
            continue;
        }

        // Создаём объект нужного класса
        UClass* FoundClass = FindObject<UClass>(this, *ClassName);
        if (!FoundClass)
        {
            // Если класс не найден, пробуем загрузить по пути
            FoundClass = LoadClass<UItemObject>(nullptr, *ClassName);
        }

        if (!FoundClass || !FoundClass->IsChildOf(UItemObject::StaticClass()))
        {
            UE_LOG(LogTemp, Warning, TEXT("Unknown class or not UItemObject: %s"), *ClassName);
            Slots.Add(NewObject<UItemObject>(this));
            continue;
        }

        UItemObject* NewItem = NewObject<UItemObject>(this, FoundClass);
        if (!NewItem)
        {
            Slots.Add(NewObject<UItemObject>(this));
            continue;
        }

        if (!NewItem->DeserializeFromJson(SlotObject))
        {
            // Если десериализация не удалась, заменяем пустым
            Slots.Add(NewObject<UItemObject>(this));
        }
        else
        {
            Slots.Add(NewItem);
        }
    }

    // Если количество слотов не совпадает с ContainerSize, добиваем или обрезаем
    while (Slots.Num() < ContainerSize)
    {
        Slots.Add(NewObject<UItemObject>(this));
    }
    if (Slots.Num() > ContainerSize)
    {
        Slots.SetNum(ContainerSize);
    }

    OnInventoryUpdated.Broadcast();
    UE_LOG(LogTemp, Log, TEXT("Inventory loaded from %s"), *FilePath);
    return true;
}

bool UTMS_InventoryComponent::HasNotFullSlotOfItem(const FName& ItemID, int32& OutIndex)
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->ItemData.ItemID == ItemID && Slots[i]->Amount < Slots[i]->ItemData.MaxAmount)
		{
			OutIndex = i;
			return true;
		}
	}
	OutIndex = -1;
	return false;
}

bool UTMS_InventoryComponent::FindFirstSlotOfType(int32& OutIndex, const FName InName)
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->ItemData.ItemID == InName)
		{
			OutIndex = i;
			return true;
		}
	}
	OutIndex = -1;
	return false;
}

bool UTMS_InventoryComponent::CreateNewEmptySlotOfType(TSubclassOf<UItemObject> InItemClass)
{
	int32 EmptySlot = -1;
	if (FindFirstSlotOfType(EmptySlot))
	{
		Slots[EmptySlot] = NewObject<UItemObject>(this, InItemClass);
		Slots[EmptySlot]->Amount = 0;
		return true;
	}
	else
	{
		return false;
	}
}

bool UTMS_InventoryComponent::TryToFill(int32 InID, int32& AmountToAdd, int32& Overflow)
{
	if (!Slots.IsValidIndex(InID)) return false;

	UItemObject* Item = Slots[InID];
	if (!Item || Item->ItemData.ItemID == NAME_None) return false;

	const int32 MaxAmount = Item->ItemData.MaxAmount;
	const int32 NewAmount = Slots[InID]->Amount + AmountToAdd;

	if (NewAmount <= MaxAmount)
	{
		Item->Amount = NewAmount;
		Overflow = 0;
		return true;
	}
	else
	{
		Overflow = NewAmount - MaxAmount;
		Item->Amount = MaxAmount;
		return false;
	}
}

UItemObject* UTMS_InventoryComponent::CreateItemObject(TSubclassOf<UItemObject> InItemClass, int32 Amount,
	EItemRarity Rarity)
{
	if (!InItemClass) return nullptr;

	UItemObject* Item = NewObject<UItemObject>(this, InItemClass);
	if (!Item) return nullptr;

	Item->Amount = Amount;

	if (UItemEquipmentObject* EquipmentItem = Cast<UItemEquipmentObject>(Item))
	{
		EquipmentItem->InitializeEquipment(Rarity);
	}

	return Item;
}

bool UTMS_InventoryComponent::AddItemToEmptySlot(UItemObject* InItem)
{
	int32 EmptySlot = -1;

	if (FindFirstSlotOfType(EmptySlot))
	{
		Slots[EmptySlot] = InItem;
		return true;
	}
	return false;
}


