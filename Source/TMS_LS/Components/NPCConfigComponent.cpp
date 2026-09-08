// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCConfigComponent.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TMS_LS/Core/Inventory/TMS_EquipmentComponent.h"
#include "TMS_LS/Core/Inventory/ItemObjects/ItemObject.h"


// Sets default values for this component's properties
UNPCConfigComponent::UNPCConfigComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNPCConfigComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (UTMS_EquipmentComponent* EC = GetOwner()->GetComponentByClass<UTMS_EquipmentComponent>())
	{
		for (auto Equipment : DefaultEquipment)
		{
			UItemObject* NewItem = NewObject<UItemObject>(GetOwner(), Equipment.Value);
			EC->AddPendingEquipment(FEquipmentProcess(NewItem, Equipment.Key, true));
		}
	}
}
