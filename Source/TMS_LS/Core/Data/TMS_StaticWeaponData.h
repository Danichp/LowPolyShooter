// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TMS_DataTypes.h"
#include "Engine/DataAsset.h"
#include "TMS_StaticWeaponData.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_StaticWeaponData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponAnimData ReloadAnims;
};
