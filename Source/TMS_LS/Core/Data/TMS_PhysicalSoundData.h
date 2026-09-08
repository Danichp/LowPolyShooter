// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Sound/SoundCue.h"
#include "Engine/DataAsset.h"
#include "TMS_PhysicalSoundData.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct TMS_LS_API FHitReactionData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USoundCue> Sound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UNiagaraSystem> NiagaraSystem;
};

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_PhysicalSoundData : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<TSoftObjectPtr<UPhysicalMaterial>, FHitReactionData> PhysicalSounds;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FHitReactionData DefaultReactionData;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USoundCue* GetPhysicalSoundBasedOnMaterial(UPhysicalMaterial* InMaterial) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UNiagaraSystem* GetPhysicalNiagaraBasedOnMaterial(UPhysicalMaterial* InMaterial) const;
};
