// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEquipment.h"


// Sets default values
AItemEquipment::AItemEquipment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemEquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAnimMontage* AItemEquipment::GetMontageByType(EEquipmentAction InAction)
{
	if (ActionMontages.Contains(InAction))
	{
		return ActionMontages[InAction];
	}
	return nullptr;
}

