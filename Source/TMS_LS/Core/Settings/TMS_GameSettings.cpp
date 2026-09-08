// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_GameSettings.h"

#include "Internationalization/Culture.h"
#include "Kismet/KismetSystemLibrary.h"

TWeakObjectPtr<UTMS_GameSettings> UTMS_GameSettings::SettingsInstance = nullptr;

UTMS_GameSettings::UTMS_GameSettings(const FObjectInitializer& ObjectInitializer)
{
	InitDefaultsSettings();
}

UTMS_GameSettings* UTMS_GameSettings::GetTMSSettings()
{
	if (!SettingsInstance.Get())
	{
		SettingsInstance = Cast<UTMS_GameSettings>(GetGameUserSettings());
		
		if (!SettingsInstance.Get())
		{
			SettingsInstance = NewObject<UTMS_GameSettings>();
			SettingsInstance->AddToRoot();
		}
		
		SettingsInstance->DefaultSettingsCopy = SettingsInstance->CreateCopy();
		SettingsInstance->LoadAllSettings();
	}
	return SettingsInstance.Get();
}

UTMS_GameSettings* UTMS_GameSettings::CreateCopy() const
{
	UTMS_GameSettings* Copy = NewObject<UTMS_GameSettings>();
	
	Copy->GraphicsPreset = GraphicsPreset;
	Copy->FPSLimit = FPSLimit;
	Copy->bAntiAliasing = bAntiAliasing;
	
	Copy->MasterVolume = MasterVolume;
	Copy->EffectsVolume = EffectsVolume;
	Copy->MusicVolume = MusicVolume;
	Copy->UIVolume = UIVolume;
	Copy->bEnableSound =bEnableSound;
	
	Copy->MouseSensitivity = MouseSensitivity;
	Copy->bInvertX = bInvertX;
	Copy->bInvertY = bInvertY;
	Copy->Language = Language;
	
	return Copy;
}

FString UTMS_GameSettings::GetQualityText(int32 Quality)
{
	switch (Quality)
	{
		case 0:
		return "Low";
		case 1:
		return "Medium";
		case 2:
		return "High";
		case 3:
		return "Ultra";
		default:
		return "Unknown";
	}
}

TArray<FString> UTMS_GameSettings::GetAvailableResolutions()
{
	TArray<FString> AResolutions;
	
	if (GEngine && GEngine->GameViewport)
	{
		TArray<FIntPoint> Resolutions;
		if (UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions))
		{
			for (const auto& Resolution : Resolutions)
			{
				AResolutions.AddUnique(
					FString::Printf(TEXT("%ix%i"), Resolution.X, Resolution.Y)
					);
			}
		}
	}
	
	if (AResolutions.Num() == 0)
	{
		AResolutions.Add(TEXT("1920x1080"));
		AResolutions.Add(TEXT("2560x1440"));
		AResolutions.Add(TEXT("3840x2160"));
		AResolutions.Add(TEXT("1280x720"));
		AResolutions.Add(TEXT("1366x768"));
		AResolutions.Add(TEXT("1600x900"));
		AResolutions.Add(TEXT("1024x768"));
		AResolutions.Add(TEXT("800x600"));
	}
	
	AResolutions.Sort([](const FString&A, const FString& B)
	{
		int32 WA, HA, WB, HB;
		FString LA, RA, LB, RB;
		
		if (A.Split(TEXT("x"), &LA, &RA) && B.Split(TEXT("x"), &LB, &RB))
		{
			WA = FCString::Atoi(*LA);
			HA = FCString::Atoi(*RA);
			WB = FCString::Atoi(*LB);
			HB = FCString::Atoi(*RB);
			
			int32 AreaA = WA * HA;
			int32 AreaB = HB * WB;
			
			return AreaA > AreaB;
		}
		return false;
	});
	
	return AResolutions;
}

TArray<FString> UTMS_GameSettings::GetAvailableLanguages()
{
	TArray<FString> Languages;
	
	const TArray<FString> ACultures;
	FInternationalization::Get().GetAvailableCultures(ACultures, false);
	for (const auto& Cult : ACultures)
	{
		FString DisplayName = Cult;
		
		if (Cult.Len() == 2 || Cult.StartsWith(TEXT("en")) || Cult.StartsWith(TEXT("ru")))
		{
			FCultureRef Culturef = FInternationalization::Get().GetCulture(Cult).ToSharedRef();
			
			DisplayName = Culturef->GetDisplayName();
			
			Languages.Add(DisplayName);
		}
	}
	
	if (Languages.Num() == 0)
	{
		Languages.AddUnique(TEXT("English"));
		Languages.AddUnique(TEXT("Russian"));
	}
	
	Languages.Sort();
	return Languages;
}

void UTMS_GameSettings::SaveAllSettings()
{
	SaveSettings();
	
	UE_LOG(LogTemp, Display, TEXT("Settings saved"));
}

void UTMS_GameSettings::LoadAllSettings()
{
	LoadSettings();
	ApplyAllSettings();
	UE_LOG(LogTemp, Display, TEXT("Settings loaded"));	
}

void UTMS_GameSettings::ApplyAllSettings()
{
	ApplyGraphicsSettings();
	ApplyAudioSettings();
	ApplyGameplaySettings();
	
	UE_LOG(LogTemp, Display, TEXT("Settings applied"));
}

void UTMS_GameSettings::ResetToDefaults()
{
	if (DefaultSettingsCopy)
	{
		GraphicsPreset = DefaultSettingsCopy->GraphicsPreset;
		FPSLimit = DefaultSettingsCopy->FPSLimit;
		bAntiAliasing = DefaultSettingsCopy->bAntiAliasing;
	
		MasterVolume = DefaultSettingsCopy->MasterVolume;
		EffectsVolume = DefaultSettingsCopy->EffectsVolume;
		MusicVolume = DefaultSettingsCopy->MusicVolume;
		UIVolume = DefaultSettingsCopy->UIVolume;
		bEnableSound = DefaultSettingsCopy->bEnableSound;
	
		MouseSensitivity = DefaultSettingsCopy->MouseSensitivity;
		bInvertX = DefaultSettingsCopy->bInvertX;
		bInvertY = DefaultSettingsCopy->bInvertY;
		Language = DefaultSettingsCopy->Language;
		
		ApplyAllSettings();
		SaveAllSettings();
		
		UE_LOG(LogTemp, Display, TEXT("Settings reset to defaults"));
	}
}

bool UTMS_GameSettings::HasChanges() const
{
	if (!DefaultSettingsCopy) return false;
	
	return GraphicsPreset != DefaultSettingsCopy->GraphicsPreset ||
	FPSLimit != DefaultSettingsCopy->FPSLimit ||
	bAntiAliasing != DefaultSettingsCopy->bAntiAliasing ||
	
	!FMath::IsNearlyEqual(MasterVolume,DefaultSettingsCopy->MasterVolume) ||
	!FMath::IsNearlyEqual(EffectsVolume, DefaultSettingsCopy->EffectsVolume) ||
	!FMath::IsNearlyEqual(MusicVolume, DefaultSettingsCopy->MusicVolume) ||
	!FMath::IsNearlyEqual(UIVolume, DefaultSettingsCopy->UIVolume) ||
	bEnableSound != DefaultSettingsCopy->bEnableSound ||
	
	!FMath::IsNearlyEqual(MouseSensitivity, DefaultSettingsCopy->MouseSensitivity) ||
	bInvertX != DefaultSettingsCopy->bInvertX ||
	bInvertY != DefaultSettingsCopy->bInvertY ||
	Language != DefaultSettingsCopy->Language ;
}

bool UTMS_GameSettings::HasChangesInCategory(ESettingsCategory Category) const
{
	if (!DefaultSettingsCopy) return false;
	
	switch (Category)
	{
	case ESettingsCategory::ESC_General:
		return !FMath::IsNearlyEqual(MouseSensitivity, DefaultSettingsCopy->MouseSensitivity) ||
				bInvertX != DefaultSettingsCopy->bInvertX ||
				bInvertY != DefaultSettingsCopy->bInvertY ||
				Language != DefaultSettingsCopy->Language ;
		
	case ESettingsCategory::ESC_Music:
		return !FMath::IsNearlyEqual(MasterVolume,DefaultSettingsCopy->MasterVolume) ||
				!FMath::IsNearlyEqual(EffectsVolume, DefaultSettingsCopy->EffectsVolume) ||
				!FMath::IsNearlyEqual(MusicVolume, DefaultSettingsCopy->MusicVolume) ||
				!FMath::IsNearlyEqual(UIVolume, DefaultSettingsCopy->UIVolume) ||
				bEnableSound != DefaultSettingsCopy->bEnableSound;

	case ESettingsCategory::ESC_Graphics:
		return GraphicsPreset != DefaultSettingsCopy->GraphicsPreset ||
				FPSLimit != DefaultSettingsCopy->FPSLimit ||
				bAntiAliasing != DefaultSettingsCopy->bAntiAliasing;
		
	default:
		return false;
	}
}

void UTMS_GameSettings::SetQualityPreset(int32 Quality)
{
	GraphicsPreset = FMath::Clamp(GraphicsPreset, 0, 3);
	ApplyQualityPreset(GraphicsPreset);
}

void UTMS_GameSettings::ApplyQualityPreset(int32 Quality)
{
	bAntiAliasing = Quality > 0;
	
	GraphicsPreset = Quality;
	ApplyScalabilitySettings();
}
#pragma region Overriden.

void UTMS_GameSettings::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();
	
	ApplyGraphicsSettings();
	ApplyAudioSettings();
	ApplyGameplaySettings();
}

void UTMS_GameSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);
	ApplyAllSettings();
	SaveAllSettings();
}

void UTMS_GameSettings::LoadSettings(bool bClearOnLoad)
{
	Super::LoadSettings(bClearOnLoad);
	
	GConfig->GetInt(TEXT("TMS_GameSettings"), TEXT("GraphicsPreset"), GraphicsPreset, GGameUserSettingsIni);
	GConfig->GetInt(TEXT("TMS_GameSettings"), TEXT("FPSLimit"), FPSLimit, GGameUserSettingsIni);
	GConfig->GetBool(TEXT("TMS_GameSettings"), TEXT("bAntiAliasing"), bAntiAliasing, GGameUserSettingsIni);
	
	GConfig->GetFloat(TEXT("TMS_GameSettings"), TEXT("MasterVolume"), MasterVolume, GGameUserSettingsIni);
	GConfig->GetFloat(TEXT("TMS_GameSettings"), TEXT("EffectsVolume"), EffectsVolume, GGameUserSettingsIni);
	GConfig->GetFloat(TEXT("TMS_GameSettings"), TEXT("MusicVolume"), MusicVolume, GGameUserSettingsIni);
	GConfig->GetFloat(TEXT("TMS_GameSettings"), TEXT("UIVolume"), UIVolume, GGameUserSettingsIni);
	GConfig->GetBool(TEXT("TMS_GameSettings"), TEXT("bEnableSound"), bEnableSound, GGameUserSettingsIni);
	
	GConfig->GetFloat(TEXT("TMS_GameSettings"), TEXT("MouseSensitivity"), MouseSensitivity, GGameUserSettingsIni);
	GConfig->GetBool(TEXT("TMS_GameSettings"), TEXT("bInvertX"), bInvertX, GGameUserSettingsIni);
	GConfig->GetBool(TEXT("TMS_GameSettings"), TEXT("bInvertY"), bInvertY, GGameUserSettingsIni);
	GConfig->GetString(TEXT("TMS_GameSettings"), TEXT("Language"), Language, GGameUserSettingsIni);
}

void UTMS_GameSettings::SaveSettings()
{
	GConfig->SetInt(TEXT("TMS_GameSettings"), TEXT("GraphicsPreset"), GraphicsPreset, GGameUserSettingsIni);
	GConfig->SetInt(TEXT("TMS_GameSettings"), TEXT("FPSLimit"), FPSLimit, GGameUserSettingsIni);
	GConfig->SetBool(TEXT("TMS_GameSettings"), TEXT("bAntiAliasing"), bAntiAliasing, GGameUserSettingsIni);
	
	GConfig->SetFloat(TEXT("TMS_GameSettings"), TEXT("MasterVolume"), MasterVolume, GGameUserSettingsIni);
	GConfig->SetFloat(TEXT("TMS_GameSettings"), TEXT("EffectsVolume"), EffectsVolume, GGameUserSettingsIni);
	GConfig->SetFloat(TEXT("TMS_GameSettings"), TEXT("MusicVolume"), MusicVolume, GGameUserSettingsIni);
	GConfig->SetFloat(TEXT("TMS_GameSettings"), TEXT("UIVolume"), UIVolume, GGameUserSettingsIni);
	GConfig->SetBool(TEXT("TMS_GameSettings"), TEXT("bEnableSound"), bEnableSound, GGameUserSettingsIni);
	
	GConfig->SetFloat(TEXT("TMS_GameSettings"), TEXT("MouseSensitivity"), MouseSensitivity, GGameUserSettingsIni);
	GConfig->SetBool(TEXT("TMS_GameSettings"), TEXT("bInvertX"), bInvertX, GGameUserSettingsIni);
	GConfig->SetBool(TEXT("TMS_GameSettings"), TEXT("bInvertY"), bInvertY, GGameUserSettingsIni);
	GConfig->SetString(TEXT("TMS_GameSettings"), TEXT("Language"), *Language, GGameUserSettingsIni);
	
	Super::SaveSettings();
	
	GConfig->Flush(false, GGameUserSettingsIni);
}

void UTMS_GameSettings::ValidateSettings()
{
	Super::ValidateSettings();
	
	GraphicsPreset = FMath::Clamp(GraphicsPreset, 0,3);
	FPSLimit = FMath::Clamp(FPSLimit, 0,60);
	
	MasterVolume = FMath::Clamp(MasterVolume, 0,1.f);
	EffectsVolume = FMath::Clamp(EffectsVolume, 0,1.f);
	MusicVolume = FMath::Clamp(MusicVolume, 0,1.f);
	UIVolume = FMath::Clamp(UIVolume, 0,1.f);
	
	MouseSensitivity = FMath::Clamp(MouseSensitivity, 0,1.f);
}
#pragma endregion Overriden

void UTMS_GameSettings::InitDefaultsSettings()
{
	GraphicsPreset = 3;
	FPSLimit = 60;
	bAntiAliasing = true;
	
	MasterVolume = 1.f;
	EffectsVolume = 1.f;
	MusicVolume = 1.f;
	UIVolume = 1.f;
	bEnableSound =true;
	
	MouseSensitivity = 1.f;
	bInvertX = false;
	bInvertY = false;
	Language = "English";
	
}

void UTMS_GameSettings::ApplyGraphicsSettings()
{
	ApplyFPSLimit();
	ApplyAntiAliasing();
	ApplyScalabilitySettings();
}

void UTMS_GameSettings::ApplyAudioSettings()
{
#if PLATFORM_WINDOWS
	//TODO audio logic
#endif
	
#if PLATFORM_ANDROID
	
#endif
	
#if !UE_BUILD_SHIPPING
	
#endif
	
}

void UTMS_GameSettings::SetResolutionFromString(const FString& ResolutionString)
{
	FString Left, Right;
	if (ResolutionString.Split(TEXT("x"), &Left, &Right))
	{
		int32 Width = FCString::Atoi(*Left);
		int32 Height = FCString::Atoi(*Right);
		if (Width > 0 && Height > 0)
		{
			SetScreenResolution(FInt32Point(Width, Height));
		}
	}
}

void UTMS_GameSettings::ApplyGameplaySettings() const
{
	if (!Language.IsEmpty())
	{
		FInternationalization::Get().SetCurrentCulture(Language);
	}
}

void UTMS_GameSettings::ApplyConsoleCommands() const
{
	if (!GEngine) return;
	
	GEngine->Exec(nullptr, *FString::Printf(TEXT("r.Antialiasing %d"), bAntiAliasing ? 4 : 0));
	GEngine->Exec(nullptr, *FString::Printf(TEXT("r.ShadowQuality %d"), GraphicsPreset));
	GEngine->Exec(nullptr, *FString::Printf(TEXT("r.TextureQuality %d"), GraphicsPreset));
	GEngine->Exec(nullptr, *FString::Printf(TEXT("r.EffectQuality %d"), GraphicsPreset));
	GEngine->Exec(nullptr, *FString::Printf(TEXT("r.PostProcessingQuality %d"), GraphicsPreset));
	GEngine->Exec(nullptr, *FString::Printf(TEXT("r.FoliageQuality %d"), GraphicsPreset));
	GEngine->Exec(nullptr, *FString::Printf(TEXT("r.ViewDistanceQuality %d"), GraphicsPreset));
}

void UTMS_GameSettings::ApplyFPSLimit() const
{
	if (GEngine)
	{
		GEngine->SetMaxFPS(FPSLimit > 0 ? FPSLimit : 0);
	}
}

void UTMS_GameSettings::ApplyAntiAliasing()
{
	if (!GEngine) return;
	
	GEngine->Exec(nullptr, *FString::Printf(TEXT("r.Antialiasing %d"), bAntiAliasing ? 4 : 0));
}

void UTMS_GameSettings::ApplyScalabilitySettings()
{
	if (!GEngine) return;
	
	Scalability::FQualityLevels QLevels;
	QLevels.SetFromSingleQualityLevel(GraphicsPreset);
	
	Scalability::SetQualityLevels(QLevels, true);
}
