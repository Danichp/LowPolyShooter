// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_LootComponent.h"
#include "TMS_LS/Core/Inventory/TMS_InventorySubsystem.h"
#include "TMS_LS/Core/Inventory/ItemObjects/ItemObject.h"
#include "TMS_LS/Utilities/TMS_DeveloperSettings.h"


UTMS_LootComponent::UTMS_LootComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTMS_LootComponent::BeginPlay()
{
	Super::BeginPlay();
	GenerateLoot();
}

#if  WITH_EDITOR
void UTMS_LootComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UTMS_LootComponent::GenerateLoot()
{
	if (!GetOwner()) return;
	for (int32 i = 0; i < LootItemsAmount; ++i)
	{
		int32 RandID = FMath::RandRange(0, PossibleLoot.Num() - 1);
		if (UTMS_InventorySubsystem* IS = GetWorld()->GetGameInstance()->GetSubsystem<UTMS_InventorySubsystem>())
		{
			if (PossibleLoot[RandID])
			{
				UItemObject* NewItem = NewObject<UItemObject>(GetOwner(), PossibleLoot[RandID]);
				if (NewItem)
				{
					NewItem->Amount = 1;
				}
				IS->AddItem(GetOwner(), NewItem);
			}
		}
	}
}

