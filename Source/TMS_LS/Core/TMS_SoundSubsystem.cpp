// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_SoundSubsystem.h"

#include "AudioDevice.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/TMS_GameSettings.h"
#include "TMS_LS/Utilities/TMS_DeveloperSettings.h"

void UTMS_SoundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTMS_SoundSubsystem::ApplyAllSettings()
{
	const float MasterVolume = UTMS_GameSettings::GetTMSSettings()->MasterVolume;
	const float MusicVolume = UTMS_GameSettings::GetTMSSettings()->MusicVolume;
	const float SFXVolume = UTMS_GameSettings::GetTMSSettings()->EffectsVolume;
	const float UIVolume = UTMS_GameSettings::GetTMSSettings()->UIVolume;
	
	FTMS_AudioPair MasterSound = UTMS_DeveloperSettings::Get()->MasterSound;
	FTMS_AudioPair MusicSound = UTMS_DeveloperSettings::Get()->MusicSound;
	FTMS_AudioPair SFXSound = UTMS_DeveloperSettings::Get()->SFXSound;
	FTMS_AudioPair UISound = UTMS_DeveloperSettings::Get()->UISound;
	
	SetClassVolume(MasterSound.SoundClass.LoadSynchronous(), MasterSound.SoundMix.LoadSynchronous(), MasterVolume);
	SetClassVolume(MusicSound.SoundClass.LoadSynchronous(), MusicSound.SoundMix.LoadSynchronous(), MusicVolume);
	SetClassVolume(SFXSound.SoundClass.LoadSynchronous(), SFXSound.SoundMix.LoadSynchronous(), SFXVolume);
	SetClassVolume(UISound.SoundClass.LoadSynchronous(), UISound.SoundMix.LoadSynchronous(), UIVolume);
}

void UTMS_SoundSubsystem::SetClassVolume(USoundClass* SoundClass, USoundMix* SoundMix, float Volume)
{
	if (!SoundMix || !SoundClass)
	{
		return;
	}
	
	SoundClass->Properties.Volume = Volume;
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SoundClass, Volume, 1.f, 1.f, true);
}
