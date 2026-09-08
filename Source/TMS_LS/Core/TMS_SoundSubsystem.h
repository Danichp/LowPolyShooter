// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TMS_SoundSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_SoundSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void ApplyAllSettings();
	
private:
	UFUNCTION()
	void SetClassVolume(USoundClass* SoundClass, USoundMix* SoundMix, float Volume);
	
};
