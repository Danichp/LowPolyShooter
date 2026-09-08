// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_PhysicalSoundData.h"

USoundCue* UTMS_PhysicalSoundData::GetPhysicalSoundBasedOnMaterial(
	UPhysicalMaterial* InMaterial) const
{
	if (InMaterial)
	{
		if (PhysicalSounds.Contains(TSoftObjectPtr<UPhysicalMaterial*>(InMaterial)))
		{
			return PhysicalSounds.Find(TSoftObjectPtr<UPhysicalMaterial*>(InMaterial))->Sound.LoadSynchronous();
		}
	}
	return nullptr;
}

UNiagaraSystem* UTMS_PhysicalSoundData::GetPhysicalNiagaraBasedOnMaterial(
	UPhysicalMaterial* InMaterial) const
{
	if (InMaterial)
	{
		if (PhysicalSounds.Contains(TSoftObjectPtr<UPhysicalMaterial*>(InMaterial)))
		{
			return PhysicalSounds.Find(TSoftObjectPtr<UPhysicalMaterial*>(InMaterial))->NiagaraSystem.LoadSynchronous();
		}
	}
	return nullptr;
}
