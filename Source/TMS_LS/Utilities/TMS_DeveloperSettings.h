// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TMS_LS/Core/Data/TMS_DataTypes.h"
#include "Sound/SoundCue.h"
#include "TMS_DeveloperSettings.generated.h"

USTRUCT(BlueprintType)
struct FTMS_AudioPair
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundMix> SoundMix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundClass> SoundClass;
};

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, Displayname = "TMS_DeveloperSettings")
class TMS_LS_API UTMS_DeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UTMS_DeveloperSettings(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	static const UTMS_DeveloperSettings* Get();

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	TMap<TEnumAsByte<ETeamType>, FTeamAffiliation> TeamsAttitudes;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TMap<TSoftObjectPtr<UPhysicalMaterial>, TSoftObjectPtr<USoundCue>> PhysicsSoundCues;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Settings | Audio")
	FTMS_AudioPair MasterSound;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Settings | Audio")
	FTMS_AudioPair MusicSound;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Settings | Audio")
	FTMS_AudioPair SFXSound;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Settings | Audio")
	FTMS_AudioPair UISound;

};
