// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_ArmorComponent.h"

UTMS_ArmorComponent::UTMS_ArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UTMS_ArmorComponent::BeginPlay()
{
	Super::BeginPlay();	
	OnEquipmentChanged.ExecuteIfBound();
}

void UTMS_ArmorComponent::SetEquipment(UEquipmentConstuctor* Equipment)
{
	CurrentEquipment = Equipment;
	OnEquipmentChanged.ExecuteIfBound();
}

