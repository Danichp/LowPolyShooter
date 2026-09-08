// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TMS_MovementData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TMS_LS_API UTMS_MovementData : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Units = "CentimetersPerSecond"))
	float WalkSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Units = "CentimetersPerSecond"))
	float RunSpeed = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StaminaCostPerSecond = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StaminaGainPerSecond = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StaminaCooldown = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Units = "Degrees"))
	float WalkFOV = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Units = "Degrees"))
	float RunFOV = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Units="Seconds"))
	float InterpTime = 2.f;
};
