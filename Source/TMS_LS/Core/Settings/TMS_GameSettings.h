// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TMS_SettingsData.h"
#include "GameFramework/GameUserSettings.h"
#include "TMS_GameSettings.generated.h"

/**
 * 
 */
UCLASS(config = TMS_GameSettings)
class TMS_LS_API UTMS_GameSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
	UTMS_GameSettings(const FObjectInitializer& ObjectInitializer);
	
public:
	UFUNCTION(BlueprintCallable, Category = Settings)
	static UTMS_GameSettings* GetTMSSettings();
	
	UTMS_GameSettings* CreateCopy() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Settings)
	static FString GetQualityText(int32 Quality);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Settings)
	static TArray<FString> GetAvailableResolutions();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Settings)
	static TArray<FString> GetAvailableLanguages();
	
public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	int32 GraphicsPreset = 3;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	int32 FPSLimit = 60;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Graphics")
	bool bAntiAliasing = true;
	
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MasterVolume = 1.f;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float EffectsVolume = 1.f;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MusicVolume = 1.f;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float UIVolume = 1.f;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Audio")
	bool bEnableSound = true;
	
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float MouseSensitivity = 1.f;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool bInvertX = false;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool bInvertY = false;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FString Language = FString("English");
	
public:
	
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SaveAllSettings();
	
	UFUNCTION(BlueprintCallable, Category = Settings)
	void LoadAllSettings();
	
	UFUNCTION(BlueprintCallable, Category = Settings)
	void ApplyAllSettings();
	
	UFUNCTION(BlueprintCallable, Category = Settings)
	void ResetToDefaults();
	
	UFUNCTION(BlueprintCallable, Category = Settings)
	bool HasChanges() const;
	
	UFUNCTION(BlueprintCallable, Category = Settings)
	bool HasChangesInCategory(ESettingsCategory Category) const;
	
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetQualityPreset(int32 Quality);
	
	UFUNCTION(BlueprintCallable, Category = Settings)
	void ApplyQualityPreset(int32 Quality);
	
	virtual void ApplyNonResolutionSettings() override;
	virtual void ApplySettings(bool bCheckForCommandLineOverrides = true) override;
	virtual void LoadSettings(bool bClearOnLoad = false) override;
	virtual void SaveSettings() override;
	virtual void ValidateSettings() override;
	
	void SetResolutionFromString(const FString& ResolutionString);
	
protected:
	
	UPROPERTY(Transient)
	UTMS_GameSettings* DefaultSettingsCopy;
	
	void InitDefaultsSettings();
	
	void ApplyGraphicsSettings();
	
	void ApplyAudioSettings();

	void ApplyGameplaySettings() const;
	
private:
	static TWeakObjectPtr<UTMS_GameSettings> SettingsInstance;
	
	void ApplyConsoleCommands() const;
	
	void ApplyFPSLimit() const;
	
	void ApplyAntiAliasing();
	
	void ApplyScalabilitySettings();
};
