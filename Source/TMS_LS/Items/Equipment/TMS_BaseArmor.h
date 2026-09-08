// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentConstuctor.h"
#include "TMS_LS/Core/Inventory/Actors/ItemEquipment.h"
#include "TMS_BaseArmor.generated.h"

UCLASS()
class TMS_LS_API ATMS_BaseArmor : public AItemEquipment
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATMS_BaseArmor();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UEquipmentConstuctor> EquipmentData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Armor = 100;

protected:
	
	virtual void BeginPlay() override;
};
