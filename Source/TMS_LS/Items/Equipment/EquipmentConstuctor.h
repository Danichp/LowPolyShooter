// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipmentConstuctor.generated.h"

UENUM(BlueprintType)
enum class EEquipmentLayer : uint8
{
	EEL_Helmet,
	EEL_Head,
	EEL_Body,
	EEL_Hands,
	EEL_Legs,
	EEL_Boots,
	EEL_MAX UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class TMS_LS_API UEquipmentConstuctor : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EEquipmentLayer, TSoftObjectPtr<USkeletalMesh>> MeshesByLayer;
};
