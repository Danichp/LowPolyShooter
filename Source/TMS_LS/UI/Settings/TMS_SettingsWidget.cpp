// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_SettingsWidget.h"

#include "TMS_ButtonTemplate.h"
#include "TMS_CheckboxTemplate.h"
#include "TMS_SliderTemplate.h"
#include "Components/ComboBoxString.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "TMS_LS/Core/TMS_SoundSubsystem.h"
#include "TMS_LS/Core/Settings/TMS_GameSettings.h"

void UTMS_SettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Settings = UTMS_GameSettings::GetTMSSettings();
	
	if (Settings)
	{
		PendingSettings = Settings->CreateCopy();
	}
	
	check(GraphicsTabButton);
	GraphicsTabButton->OnButtonClicked.AddDynamic(this, &UTMS_SettingsWidget::OnGraphicsTabClicked);
	
	check(AudioTabButton);
	AudioTabButton->OnButtonClicked.AddDynamic(this, &UTMS_SettingsWidget::OnAudioTabClicked);
	
	check(GameplayTabButton);
	GameplayTabButton->OnButtonClicked.AddDynamic(this, &UTMS_SettingsWidget::OnGameplayTabClicked);
	
	check(ApplyButton)
	ApplyButton->OnButtonClicked.AddDynamic(this, &UTMS_SettingsWidget::OnApplyButtonClicked);
	
	check(ResetButton);
	ResetButton->OnButtonClicked.AddDynamic(this, &UTMS_SettingsWidget::OnResetButtonClicked);
	
	InitWidget();
}

void UTMS_SettingsWidget::NativeDestruct()
{
	Super::NativeDestruct();
	UnbindWidgets();
	GraphicsTabButton->OnButtonClicked.RemoveDynamic(this, &UTMS_SettingsWidget::OnGraphicsTabClicked);
	AudioTabButton->OnButtonClicked.RemoveDynamic(this, &UTMS_SettingsWidget::OnAudioTabClicked);
	GameplayTabButton->OnButtonClicked.RemoveDynamic(this, &UTMS_SettingsWidget::OnGameplayTabClicked);
	ApplyButton->OnButtonClicked.RemoveDynamic(this, &UTMS_SettingsWidget::OnApplyButtonClicked);
	ResetButton->OnButtonClicked.RemoveDynamic(this, &UTMS_SettingsWidget::OnResetButtonClicked);
	
	Super::NativeDestruct();
}

void UTMS_SettingsWidget::OnGraphicsTabClicked()
{
	if (CategorySwitcher)
	{
		CategorySwitcher->SetActiveWidgetIndex(0);
		CurTab = 0;
		if (TabTitle)
			TabTitle->SetText(FText::FromString(TEXT("Graphics Settings")));
	}
	SaveSettings();
}

void UTMS_SettingsWidget::OnAudioTabClicked()
{
	if (CategorySwitcher)
	{
		CategorySwitcher->SetActiveWidgetIndex(1);
		CurTab = 1;
		if (TabTitle)
			TabTitle->SetText(FText::FromString(TEXT("Audio Settings")));
	}
	SaveSettings();
}

void UTMS_SettingsWidget::OnGameplayTabClicked()
{
	if (CategorySwitcher)
	{
		CategorySwitcher->SetActiveWidgetIndex(2);
		CurTab = 2;
		if (TabTitle)
			TabTitle->SetText(FText::FromString(TEXT("Gameplay Settings")));
	}
	SaveSettings();
}

void UTMS_SettingsWidget::OnGraphicPresetChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SaveSettings();
}

void UTMS_SettingsWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SaveSettings();
}

void UTMS_SettingsWidget::OnFullscreenChanged(bool bNewValue)
{
	SaveSettings();
}

void UTMS_SettingsWidget::OnAntiAliasingChanged(bool bNewValue)
{
	SaveSettings();
}

void UTMS_SettingsWidget::OnFPSSelectorChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SaveSettings();
}

void UTMS_SettingsWidget::OnMasterSoundChanged(float NewValue)
{
	UpdateSliderLabels();
	SaveSettings();
}

void UTMS_SettingsWidget::OnMusicSoundChanged(float NewValue)
{
	UpdateSliderLabels();
	SaveSettings();
}

void UTMS_SettingsWidget::OnSFXSoundChanged(float NewValue)
{
	UpdateSliderLabels();
	SaveSettings();
}

void UTMS_SettingsWidget::OnUISoundChanged(float NewValue)
{
	UpdateSliderLabels();
	SaveSettings();
}

void UTMS_SettingsWidget::OnEnableSoundChanged(bool bNewValue)
{
	SaveSettings();
}

void UTMS_SettingsWidget::OnMouseSensitivityChanged(float NewValue)
{
	UpdateSliderLabels();
	SaveSettings();
}

void UTMS_SettingsWidget::OnInvertXChanged(bool bNewValue)
{
	SaveSettings();
}

void UTMS_SettingsWidget::OnInvertYChanged(bool bNewValue)
{
	SaveSettings();
}

void UTMS_SettingsWidget::OnLanguageChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UpdateSliderLabels();
	SaveSettings();
}

void UTMS_SettingsWidget::OnApplyButtonClicked()
{
	SaveSettings();
	ApplyPendingSettings();
	ApplyAudioSettings();
}

void UTMS_SettingsWidget::OnResetButtonClicked()
{
	if (Settings)
	{
		Settings->ResetToDefaults();
		PendingSettings = Settings->CreateCopy();
		UnbindWidgets();
		LoadSettingsToUI();
		UpdateChangesIndicator();
		BindWidgets();
	}
}

void UTMS_SettingsWidget::InitWidget()
{
	InitComboBoxes();
	
	LoadSettingsToUI();
	
	if (CategorySwitcher)
	{
		CategorySwitcher->SetActiveWidgetIndex(0);
		CurTab = 0;
	}
	
	UpdateChangesIndicator();
	
	BindWidgets();
}

void UTMS_SettingsWidget::BindWidgets()
{
	MasterSoundSlider->OnSliderValueChanged.AddDynamic(this, &UTMS_SettingsWidget::OnMasterSoundChanged);
	EffectsSoundSlider->OnSliderValueChanged.AddDynamic(this, &UTMS_SettingsWidget::OnMusicSoundChanged);
	MusicSoundSlider->OnSliderValueChanged.AddDynamic(this, &UTMS_SettingsWidget::OnSFXSoundChanged);
	UISoundSlider->OnSliderValueChanged.AddDynamic(this, &UTMS_SettingsWidget::OnUISoundChanged);
	
	MouseSensitivitySlider->OnSliderValueChanged.AddDynamic(this, &UTMS_SettingsWidget::OnMouseSensitivityChanged);
	
	ResolutionPresetComboBox->OnSelectionChanged.AddDynamic(this, &UTMS_SettingsWidget::OnResolutionChanged);
	QualityPresetComboBox->OnSelectionChanged.AddDynamic(this, &UTMS_SettingsWidget::OnGraphicPresetChanged);
	FPSComboBox->OnSelectionChanged.AddDynamic(this, &UTMS_SettingsWidget::OnFPSSelectorChanged);
	
	LanguageComboBox->OnSelectionChanged.AddDynamic(this, &UTMS_SettingsWidget::OnLanguageChanged);
	
	check(FullScreenCheckbox)
	FullScreenCheckbox->OnCheckboxStateChanged.AddDynamic(this, &UTMS_SettingsWidget::OnFullscreenChanged);
	
	check(AntiAliasingCheckbox)
	AntiAliasingCheckbox->OnCheckboxStateChanged.AddDynamic(this, &UTMS_SettingsWidget::OnAntiAliasingChanged);
	
	check(EnableSoundsCheckbox)
	EnableSoundsCheckbox->OnCheckboxStateChanged.AddDynamic(this, &UTMS_SettingsWidget::OnEnableSoundChanged);
	
	check(InvertXCheckbox)
	InvertXCheckbox->OnCheckboxStateChanged.AddDynamic(this, &UTMS_SettingsWidget::OnInvertXChanged);
	
	check(InvertYCheckbox)
	InvertYCheckbox->OnCheckboxStateChanged.AddDynamic(this, &UTMS_SettingsWidget::OnInvertYChanged);
}

void UTMS_SettingsWidget::UnbindWidgets()
{
	MasterSoundSlider->OnSliderValueChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnMasterSoundChanged);
	EffectsSoundSlider->OnSliderValueChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnMusicSoundChanged);
	MusicSoundSlider->OnSliderValueChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnSFXSoundChanged);
	UISoundSlider->OnSliderValueChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnUISoundChanged);
	
	MouseSensitivitySlider->OnSliderValueChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnMouseSensitivityChanged);
	
	ResolutionPresetComboBox->OnSelectionChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnResolutionChanged);
	QualityPresetComboBox->OnSelectionChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnGraphicPresetChanged);
	FPSComboBox->OnSelectionChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnFPSSelectorChanged);
	
	LanguageComboBox->OnSelectionChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnLanguageChanged);
	
	check(FullScreenCheckbox)
	FullScreenCheckbox->OnCheckboxStateChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnFullscreenChanged);
	
	check(AntiAliasingCheckbox)
	AntiAliasingCheckbox->OnCheckboxStateChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnAntiAliasingChanged);
	
	check(EnableSoundsCheckbox)
	EnableSoundsCheckbox->OnCheckboxStateChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnEnableSoundChanged);
	
	check(InvertXCheckbox)
	InvertXCheckbox->OnCheckboxStateChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnInvertXChanged);
	
	check(InvertYCheckbox)
	InvertYCheckbox->OnCheckboxStateChanged.RemoveDynamic(this, &UTMS_SettingsWidget::OnInvertYChanged);
}

void UTMS_SettingsWidget::LoadSettingsToUI()
{
	if (!PendingSettings) return;
	
	if (ResolutionPresetComboBox)
	{
		FString ResolutionString = FString::Printf(TEXT("%dx%d"),
			PendingSettings->GetScreenResolution().X,
			PendingSettings->GetScreenResolution().Y);
		ResolutionPresetComboBox->SetSelectedOption(ResolutionString);
	}
	
	if (FullScreenCheckbox)
	{
		FullScreenCheckbox->SetCheckboxValue(PendingSettings->GetFullscreenMode() == EWindowMode::Fullscreen);
	}
	
	if (QualityPresetComboBox)
	{
		FString PresetText = GetQualityText(PendingSettings->GraphicsPreset);
		QualityPresetComboBox->SetSelectedOption(PresetText);
	}
	
	if (FPSComboBox)
	{
		int32 FPSLimit = PendingSettings->FPSLimit;
		FString FPSStr = FPSLimit > 0 ? FString::FromInt(FPSLimit) : TEXT("Unlimited");
		FPSComboBox->SetSelectedOption(FPSStr);
	}
	
	if (AntiAliasingCheckbox)
	{
		AntiAliasingCheckbox->SetCheckboxValue(PendingSettings->bAntiAliasing);
	}
	
	if (MasterSoundSlider)
	{
		MasterSoundSlider->SetSliderValue(PendingSettings->MasterVolume);
	}
	
	if (MusicSoundSlider)
	{
		MusicSoundSlider->SetSliderValue(PendingSettings->MusicVolume);
	}
	
	if (EffectsSoundSlider)
	{
		EffectsSoundSlider->SetSliderValue(PendingSettings->EffectsVolume);
	}
	
	if (UISoundSlider)
	{
		UISoundSlider->SetSliderValue(PendingSettings->UIVolume);
	}
	
	if (EnableSoundsCheckbox)
	{
		EnableSoundsCheckbox->SetCheckboxValue(PendingSettings->bEnableSound);
	}
	
	
	if (MouseSensitivitySlider)
	{
		MouseSensitivitySlider->SetSliderValue(PendingSettings->MouseSensitivity);
	}
	
	if (InvertXCheckbox)
	{
		InvertXCheckbox->SetCheckboxValue(PendingSettings->bInvertX);
	}
	
	if (InvertYCheckbox)
	{
		InvertYCheckbox->SetCheckboxValue(PendingSettings->bInvertY);
	}
	
	if (LanguageComboBox)
	{
		LanguageComboBox->SetSelectedOption(PendingSettings->Language);
	}
	
	UpdateSliderLabels();
}

void UTMS_SettingsWidget::SaveSettings()
{
	if (!PendingSettings) return;
	
	if (ResolutionPresetComboBox)
	{
		FString Selected = ResolutionPresetComboBox->GetSelectedOption();
		PendingSettings->SetResolutionFromString(Selected);
	}
	
	if (FullScreenCheckbox)
	{
		PendingSettings->SetFullscreenMode(FullScreenCheckbox->GetCheckboxValue() ? EWindowMode::Fullscreen : EWindowMode::Windowed);
	}
	
	if (QualityPresetComboBox)
	{
		FString Selected = QualityPresetComboBox->GetSelectedOption();
		int32 Preset = 0;
		if (Selected == "Low")
		{
			Preset = 0;
		}
		else if (Selected == "Medium")
		{
			Preset = 1;
		}
		else if (Selected == "High")
		{
			Preset = 2;
		}
		else if (Selected == "Ultra")
		{
			Preset = 3;
		}
		
		PendingSettings->GraphicsPreset = Preset;
		PendingSettings->ApplyQualityPreset(Preset);
	}
	
	if (FPSComboBox)
	{
		FString Selected = FPSComboBox->GetSelectedOption();
		
		int32 FPS = 0;
		if (Selected != TEXT("Unlimited"))
			FPS = FCString::Atoi(*Selected);
		PendingSettings->FPSLimit = FPS;
	}
	
	if (AntiAliasingCheckbox)
	{
		PendingSettings->bAntiAliasing = AntiAliasingCheckbox->GetCheckboxValue();
	}
	
	
	if (MasterSoundSlider)
	{
		PendingSettings->MasterVolume = (MasterSoundSlider->GetSliderValue());
	}
	
	if (MusicSoundSlider)
	{
		PendingSettings->MusicVolume = (MusicSoundSlider->GetSliderValue());
	}
	
	if (EffectsSoundSlider)
	{
		PendingSettings->EffectsVolume = (EffectsSoundSlider->GetSliderValue());
	}
	
	if (UISoundSlider)
	{
		PendingSettings->UIVolume = (UISoundSlider->GetSliderValue());
	}
	
	if (EnableSoundsCheckbox)
	{
		PendingSettings->bEnableSound = EnableSoundsCheckbox->GetCheckboxValue();
	}
	
	UpdateChangesIndicator();
}

void UTMS_SettingsWidget::ApplyPendingSettings()
{
	if (!Settings || !PendingSettings) return;

	Settings->GraphicsPreset = PendingSettings->GraphicsPreset;
	Settings->FPSLimit = PendingSettings->FPSLimit;
	Settings->bAntiAliasing = PendingSettings->bAntiAliasing;

	Settings->MasterVolume = PendingSettings->MasterVolume;
	Settings->MusicVolume = PendingSettings->MusicVolume;
	Settings->EffectsVolume = PendingSettings->EffectsVolume;
	Settings->UIVolume = PendingSettings->UIVolume;
	Settings->bEnableSound = PendingSettings->bEnableSound;

	Settings->MouseSensitivity = PendingSettings->MouseSensitivity;
	Settings->bInvertY = PendingSettings->bInvertY;
	Settings->bInvertX = PendingSettings->bInvertX;
	Settings->Language = PendingSettings->Language;

	Settings->ApplyAllSettings();
	Settings->SaveSettings();
	
	if (Settings)
	{
		PendingSettings = Settings->CreateCopy();
	}

	UpdateChangesIndicator();
}

void UTMS_SettingsWidget::ApplyAudioSettings()
{
	if (!GetWorld() || !GetWorld()->GetGameInstance())
	{
		return;
	}
	if (UTMS_SoundSubsystem* TSS = GetWorld()->GetGameInstance()->GetSubsystem<UTMS_SoundSubsystem>())
	{
		TSS->ApplyAllSettings();
	}
}

void UTMS_SettingsWidget::RevertPendingSettings()
{
	if (!Settings) return;

	// Восстанавливаем из текущих настроек
	PendingSettings = Settings->CreateCopy();
    
	// Обновляем UI
	LoadSettingsToUI();
    
	UpdateChangesIndicator();
}

void UTMS_SettingsWidget::UpdateChangesIndicator()
{
	if (!ChangesIndicator) return;

	bool bHasChanges = false;
	if (Settings && PendingSettings)
	{
		bHasChanges = Settings->GraphicsPreset != PendingSettings->GraphicsPreset ||
					  Settings->FPSLimit != PendingSettings->FPSLimit ||
					  Settings->bAntiAliasing != PendingSettings->bAntiAliasing ||
					  !FMath::IsNearlyEqual(Settings->MasterVolume, PendingSettings->MasterVolume) ||
					  !FMath::IsNearlyEqual(Settings->MouseSensitivity, PendingSettings->MouseSensitivity);
	}

	// Меняем цвет индикатора
	FLinearColor IndicatorColor = bHasChanges ? FLinearColor::Yellow : FLinearColor::Green;
	ChangesIndicator->SetColorAndOpacity(IndicatorColor);
}

void UTMS_SettingsWidget::UpdateSliderLabels()
{
	if (MasterSoundTextBlock)
		MasterSoundTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), 
			FMath::RoundToInt(MasterSoundSlider->GetSliderValue() * 100))));
    
	if (MusicSoundTextBlock)
		MusicSoundTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), 
			FMath::RoundToInt(MusicSoundSlider->GetSliderValue() * 100))));
    
	if (EffectsSoundTextBlock)
		EffectsSoundTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), 
			FMath::RoundToInt(EffectsSoundSlider->GetSliderValue() * 100))));
    
	if (UISoundTextBlock)
		UISoundTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), 
			FMath::RoundToInt(UISoundSlider->GetSliderValue() * 100))));
}

FString UTMS_SettingsWidget::GetQualityText(int32 Quality)
{
	switch (Quality)
	{
	case 0: 
		return TEXT("Low");
	case 1: 
		return TEXT("Medium");
	case 2: 
		return TEXT("High");
	case 3: 
		return TEXT("Ultra");
	default: 
		return TEXT("Unknown");
	}
}

void UTMS_SettingsWidget::InitComboBoxes()
{
	check(ResolutionPresetComboBox)
	TArray<FString> AvailableResolutions = UTMS_GameSettings::GetAvailableResolutions();
	for (const auto& Resolution : AvailableResolutions)
	{
		ResolutionPresetComboBox->AddOption(Resolution);
	}
	
	check(QualityPresetComboBox)
	QualityPresetComboBox->AddOption(TEXT("Low"));
	QualityPresetComboBox->AddOption(TEXT("Medium"));
	QualityPresetComboBox->AddOption(TEXT("High"));
	QualityPresetComboBox->AddOption(TEXT("Ultra"));
	
	check(FPSComboBox)
	FPSComboBox->AddOption(TEXT("Unlimited"));
	FPSComboBox->AddOption(TEXT("30"));
	FPSComboBox->AddOption(TEXT("60"));
	FPSComboBox->AddOption(TEXT("120"));
	FPSComboBox->AddOption(TEXT("144"));
	FPSComboBox->AddOption(TEXT("240"));
	
	check(LanguageComboBox)
	TArray<FString> AvailableLanguages = UTMS_GameSettings::GetAvailableLanguages();
	for (const auto& Language : AvailableLanguages)
	{
		LanguageComboBox->AddOption(Language);
	}
}
