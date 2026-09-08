// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TMS_LoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "TMS LoadingScreen"))
class TMS_LOADINGSCREEN_API UTMS_LoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UTMS_LoadingScreenSettings(const FObjectInitializer& initializer);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static const UTMS_LoadingScreenSettings* Get();
	
	UPROPERTY(Config, EditAnywhere)
	FSoftObjectPath BGTexture;
	
	UPROPERTY(Config, EditAnywhere)
	float MinimumDisplayTime = 2.f;
	
};
