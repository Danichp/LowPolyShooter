// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TMS_WeaponData.generated.h"

struct FWeaponRuntimeStats;
/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_WeaponData : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable)
	FWeaponRuntimeStats GenerateStats();
};
