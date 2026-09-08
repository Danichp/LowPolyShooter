// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_EquipmentComponent.h"

#include "TMS_InventoryComponent.h"
#include "TMS_InventorySubsystem.h"
#include "GameFramework/Character.h"
#include "ItemObjects/ItemObject.h"
#include "TMS_LS/Components/TMS_ArmorComponent.h"
#include "TMS_LS/Components/TMS_WeaponComponent.h"
#include "TMS_LS/Core/TMS_BaseCharacter.h"
#include "TMS_LS/Items/Equipment/TMS_BaseArmor.h"

UTMS_EquipmentComponent::UTMS_EquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	for (EEquipmentType ET : TEnumRange<EEquipmentType>())
	{
		EquipmentObjects.FindOrAdd(ET);
		EquipmentActors.FindOrAdd(ET);
	}
}

void UTMS_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	for (EEquipmentType Type : TEnumRange<EEquipmentType>())
	{
		if (UItemObject* Item = *EquipmentObjects.Find(Type))
		{
			if (Item->ItemData.ItemID != NAME_None)
			{
				FEquipmentProcess EquipmentProcess(Item, Type, true);
				AddPendingEquipment(EquipmentProcess);
			}
		}
	}
	ATMS_BaseCharacter* OwnerPawn = Cast<ATMS_BaseCharacter>(GetOwner());
	if (!OwnerPawn) return;
	OwnerPawn->OnEnemyDestroyed.AddDynamic(this, &UTMS_EquipmentComponent::DestroyedEquipment);
}

void UTMS_EquipmentComponent::AddPendingEquipment(const FEquipmentProcess& InEP)
{
	EquipmentProcesses.Enqueue(InEP);
	if (!CurrentEquipmentProcess.IsValid())
	{
		NextPendingEquipment();
	}
}

void UTMS_EquipmentComponent::GetEquipmentBySlot(EEquipmentType InSlot, UItemObject*& OutItem,
	AItemEquipment*& OutEquipmentActor)
{
	if (!EquipmentObjects.Contains(InSlot) || !EquipmentActors.Contains(InSlot)) return;
	OutItem = EquipmentObjects[InSlot];
	OutEquipmentActor = EquipmentActors[InSlot];
}

// bool UTMS_EquipmentComponent::SaveEquipmentToFile(const FString& FilePath) const
// {
// 	FString Directory = FPaths::GetPath(FilePath);
// 	IFileManager::Get().MakeDirectory(*Directory, true);
//
// 	FString JsonString = SerializeToJson();
//
// 	if (JsonString.IsEmpty())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Failed to serialize data to Json"));
// 		return false;
// 	}
//
// 	bool bSaved = FFileHelper::SaveStringToFile(JsonString, *FilePath);
//
// 	if (bSaved)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Successfully saved data to %s"), *FilePath);
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Failed to save data to %s"), *FilePath);
// 	}
//
// 	return bSaved;
// }
//
// bool UTMS_EquipmentComponent::LoadEquipmentFromFile(const FString& FilePath)
// {
// 	if (!FPaths::FileExists(FilePath))
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("No save file on path %s"), *FilePath);
// 		return false;
// 	}
//
// 	FString JsonString;
// 	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Failed to load save file on path %s"), *FilePath);
// 		return false;
// 	}
//
// 	bool bLoaded = DeserializeFromJson(JsonString);
//
// 	if (bLoaded)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Successfully loaded save file on path %s"), *FilePath);
//
//
// 		OnEquipmentUpdated.Broadcast();
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Failed to deserialize inventory from %s"), *FilePath);
// 	}
//
// 	return bLoaded;
// }

void UTMS_EquipmentComponent::NextPendingEquipment()
{
	OnSlotUpdate.Broadcast(CurrentEquipmentProcess.Slot);
	CurrentEquipmentProcess = FEquipmentProcess();
	if (EquipmentProcesses.IsEmpty()) return;
	EquipmentProcesses.Dequeue(CurrentEquipmentProcess);
	EquipmentProcesses.Pop();

	if (!CurrentEquipmentProcess.IsValid()) NextPendingEquipment();

	if (CurrentEquipmentProcess.bEquipment)
	{
		EquipSlot(CurrentEquipmentProcess.Slot, CurrentEquipmentProcess.ItemData);
	}
	else
	{
		UnequipSlot(CurrentEquipmentProcess.Slot);
	}
}

void UTMS_EquipmentComponent::EquipSlot(EEquipmentType InSlot, UItemObject* InItem)
{
	if (!GetWorld()) return;
	OnBeginEquip.Broadcast(InSlot);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	if (InItem->ItemData.ItemID == NAME_None)
	{
		FinishEquip();
		return;
	}

	FItemData ItemData;
	if (UTMS_InventorySubsystem* IS = GetWorld()->GetGameInstance()->GetSubsystem<UTMS_InventorySubsystem>())
	{
		IS->GetItemData(InItem->ItemData.ItemID, ItemData);
	}
	if (ItemData.ItemID == NAME_None)
	{
		FinishEquip();
		return;
	}

	AItemEquipment* EquipmentActor = GetWorld()->SpawnActor<AItemEquipment>(ItemData.ItemClass.Get(), SpawnParams);

	if (!EquipmentActor)
	{
		FinishEquip();
		return;
	}

	EquipmentObjects.Emplace(InSlot, InItem);
	EquipmentActors.Emplace(InSlot, EquipmentActor);

	MoveCurrentItemInInventory();

	ProcessEquip(InSlot);
}

void UTMS_EquipmentComponent::UnequipSlot(EEquipmentType InSlot)
{
	if (!EquipmentActors[InSlot]) return;

	OnBeginUnequip.Broadcast(InSlot);
	ProcessUnequip(InSlot);
}

void UTMS_EquipmentComponent::ProcessEquip(EEquipmentType InSlot)
{
	AItemEquipment* ItemEquipment = EquipmentActors[InSlot];
	if (!ItemEquipment || !GetWorld() || !GetOwner())
	{
		FinishEquip();
		return;
	}

	FAttachmentTransformRules Rules = FAttachmentTransformRules::SnapToTargetIncludingScale;

	ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (!Player)
	{
		FinishEquip();
		return;
	}

	ItemEquipment->AttachToComponent(Player->GetMesh(), Rules, ItemEquipment->SocketName);
	float AnimLength = 1.f;
	if (UAnimMontage* EquipAnim = ItemEquipment->GetMontageByType(EEquipmentAction::EEA_Equip))
	{
		AnimLength = Player->PlayAnimMontage(EquipAnim);
	}

	GetWorld()->GetTimerManager().SetTimer(EquipmentHandle,
		this, &UTMS_EquipmentComponent::FinishEquip,
		AnimLength, false);
}

void UTMS_EquipmentComponent::ProcessUnequip(EEquipmentType InSlot)
{
	AItemEquipment* EquipmentItem = EquipmentActors[InSlot];
	if (!GetWorld() || !GetOwner() || !EquipmentItem)
	{
		FinishUnequip();
		return;
	}

	UAnimMontage* EquipAnim = EquipmentItem->GetMontageByType(EEquipmentAction::EEA_Unequip);
	float AnimLength = EquipAnim ? EquipAnim->GetPlayLength() : 1.6f;
	if (EquipAnim)
	{
		if (ACharacter* Player = Cast<ACharacter>(GetOwner()))
		{
			Player->GetMesh()->GetAnimInstance()->Montage_Play(EquipAnim);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(EquipmentHandle, this,
		&ThisClass::FinishUnequip, AnimLength, false);
}

void UTMS_EquipmentComponent::FinishEquip()
{
	switch(CurrentEquipmentProcess.Slot)
	{
	case EEquipmentType::EET_Main:
		if (UTMS_WeaponComponent* WC = GetOwner()->GetComponentByClass<UTMS_WeaponComponent>())
		{
			WC->SetCurrentWeapon(Cast<ATMS_BaseWeapon>(EquipmentActors[CurrentEquipmentProcess.Slot]));
		}
		break;
	case EEquipmentType::EET_Armor:
		if (UTMS_ArmorComponent* AC = GetOwner()->GetComponentByClass<UTMS_ArmorComponent>())
		{
			if (ATMS_BaseArmor* ArmorItem = Cast<ATMS_BaseArmor>(EquipmentActors[CurrentEquipmentProcess.Slot]))
			{
				AC->SetEquipment(ArmorItem->EquipmentData);
			}
		}
		break;
	default:
		break;
	}

	OnFinishEquip.Broadcast(CurrentEquipmentProcess.Slot);
	OnEquipmentUpdated.Broadcast();
	NextPendingEquipment();
}

void UTMS_EquipmentComponent::FinishUnequip()
{

	switch(CurrentEquipmentProcess.Slot)
	{
	case EEquipmentType::EET_Main:
		if (UTMS_WeaponComponent* WC = GetOwner()->GetComponentByClass<UTMS_WeaponComponent>())
		{
			WC->SetCurrentWeapon(nullptr);
		}
		break;
	case EEquipmentType::EET_Armor:
		if (UTMS_ArmorComponent* AC = GetOwner()->GetComponentByClass<UTMS_ArmorComponent>())
		{
			AC->SetEquipment(nullptr);
		}
		break;
	default:
		break;
	}

	if (AActor* EquipActor = EquipmentActors[CurrentEquipmentProcess.Slot])
	{
		EquipActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquipActor->Destroy();
	}

	MoveCurrentItemInInventory();

	EquipmentActors.Emplace(CurrentEquipmentProcess.Slot);
	EquipmentObjects.Emplace(CurrentEquipmentProcess.Slot);

	OnFinishUnequip.Broadcast(CurrentEquipmentProcess.Slot);
	OnEquipmentUpdated.Broadcast();
	NextPendingEquipment();
}

void UTMS_EquipmentComponent::MoveCurrentItemInInventory()
{
	if (!CurrentEquipmentProcess.IsValid()) return;

	if (ACharacter* Player = Cast<ACharacter>(GetOwner()))
	{
		if (UTMS_InventoryComponent* IC =
			Player->GetComponentByClass<UTMS_InventoryComponent>())
		{
			UItemObject* Item = EquipmentObjects[CurrentEquipmentProcess.Slot];
			if (Item->ItemData.ItemID == NAME_None) return;

			if (CurrentEquipmentProcess.bEquipment)
			{
				IC->RemoveItemAsObject(Item);
			}
			else
			{
				IC->AddItemAsObject(Item);
			}
		}
	}
}

void UTMS_EquipmentComponent::DestroyedEquipment()
{
	for ( auto It = EquipmentActors.CreateIterator(); It; ++It )
		if (AItemEquipment* ItemEquipment = It.Value())
		{
			ItemEquipment->SetLifeSpan(5.0f);
		}
}

// FString UTMS_EquipmentComponent::SerializeToJson() const
// {
//
// 	TSharedPtr<FJsonObject> JObject = MakeShareable(new FJsonObject);
//
// 	for (const auto& Pair : EquipmentObjects)
// 	{
// 		FString KeyString = UEnum::GetValueAsString(Pair.Key);
//
// 		TSharedPtr<FJsonObject> SlotJson = Pair.Value.AsJsonObject();
//
// 		if (SlotJson.IsValid())
// 		{
// 			JObject->SetObjectField(KeyString, SlotJson);
// 		}
// 	}
//
// 	FString OutString;
// 	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutString);
// 	if (!FJsonSerializer::Serialize(JObject.ToSharedRef(), Writer))
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Failed to serialize equipment to JSON"));
// 		return FString();
// 	}
// 	return OutString;
// }
//
// bool UTMS_EquipmentComponent::DeserializeFromJson(const FString& InJsonString)
// {
// 	TSharedPtr<FJsonObject> JObject;
//
// 	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InJsonString);
// 	if (!FJsonSerializer::Deserialize(JsonReader, JObject) || !JObject.IsValid())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON"));
// 		return false;
// 	}
//
// 	EquipmentObjects.Empty();
// 	UEnum* EnumPtr = StaticEnum<EEquipmentType>();
//
// 	for (const auto& Pair : JObject->Values)
// 	{
// 		const FString& KeyString = Pair.Key;
// 		const TSharedPtr<FJsonValue>& JsonValue = Pair.Value;
//
// 		int64 EnumValue = EnumPtr->GetValueByNameString(KeyString);
// 		if (EnumValue == INDEX_NONE)
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Unknown equipment type in JSON: %s"), *KeyString);
// 			continue;
// 		}
// 		EEquipmentType Type = static_cast<EEquipmentType>(EnumValue);
//
// 		const TSharedPtr<FJsonObject>* SlotObjectPtr;
// 		if (!JsonValue->TryGetObject(SlotObjectPtr) || !SlotObjectPtr->IsValid())
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("Invalid slot object for key: %s"), *KeyString);
// 			continue;
// 		}
//
// 		FItemSlotData Slot;
// 		Slot.FromJson(*SlotObjectPtr);
//
// 		EquipmentObjects.Add(Type, Slot);
// 		if (Slot.ItemID != NAME_None && Slot.Amount > 0)
// 		{
// 			FEquipmentProcess EquipmentProcess(Slot, Type, true);
// 			AddPendingEquipment(EquipmentProcess);
// 		}
// 	}
//
// 	return true;
// }
