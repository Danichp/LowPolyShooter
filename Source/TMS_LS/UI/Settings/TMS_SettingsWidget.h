// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TMS_SettingsWidget.generated.h"

class UTMS_GameSettings;
class UWidgetSwitcher;
class UTMS_ButtonTemplate;
class UTMS_CheckboxTemplate;
class UTMS_SliderTemplate;
class UComboBoxString;
class UTextBlock;
class UImage;


/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_SettingsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* CategorySwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_ButtonTemplate* GraphicsTabButton;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_ButtonTemplate* AudioTabButton;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_ButtonTemplate* GameplayTabButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TabTitle;
	
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* QualityPresetComboBox;
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResolutionPresetComboBox;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_CheckboxTemplate* FullScreenCheckbox;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_CheckboxTemplate* AntiAliasingCheckbox;
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* FPSComboBox;
	
	
	UPROPERTY(meta = (BindWidget))
	UTMS_SliderTemplate* MasterSoundSlider;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MasterSoundTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_SliderTemplate* EffectsSoundSlider;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EffectsSoundTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_SliderTemplate* MusicSoundSlider;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MusicSoundTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_SliderTemplate* UISoundSlider;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* UISoundTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_CheckboxTemplate* EnableSoundsCheckbox;
	
	
	UPROPERTY(meta = (BindWidget))
	UTMS_SliderTemplate* MouseSensitivitySlider;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_CheckboxTemplate* InvertXCheckbox;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_CheckboxTemplate* InvertYCheckbox;
	
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* LanguageComboBox;
	
	
	UPROPERTY(meta = (BindWidget))
	UTMS_ButtonTemplate* ApplyButton;
	
	UPROPERTY(meta = (BindWidget))
	UTMS_ButtonTemplate* ResetButton;
	
	UPROPERTY(meta = (BindWidget))
	UImage* ChangesIndicator;
	
public:
	UFUNCTION()
	void OnGraphicsTabClicked();
	UFUNCTION()
	void OnAudioTabClicked();
	UFUNCTION()
	void OnGameplayTabClicked();
	
	UFUNCTION()
	void OnGraphicPresetChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnFullscreenChanged(bool bNewValue);
	UFUNCTION()
	void OnAntiAliasingChanged(bool bNewValue);
	UFUNCTION()
	void OnFPSSelectorChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UFUNCTION()
	void OnMasterSoundChanged(float NewValue);
	UFUNCTION()
	void OnMusicSoundChanged(float NewValue);
	UFUNCTION()
	void OnSFXSoundChanged(float NewValue);
	UFUNCTION()
	void OnUISoundChanged(float NewValue);
	UFUNCTION()
	void OnEnableSoundChanged(bool bNewValue);
	
	UFUNCTION()
	void OnMouseSensitivityChanged(float NewValue);
	UFUNCTION()
	void OnInvertXChanged(bool bNewValue);
	UFUNCTION()
	void OnInvertYChanged(bool bNewValue);
	UFUNCTION()
	void OnLanguageChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	UFUNCTION()
	void OnApplyButtonClicked();
	UFUNCTION()
	void OnResetButtonClicked();
	
private:
	
	UPROPERTY()
	TObjectPtr<UTMS_GameSettings> Settings;
	
	UPROPERTY()
	TObjectPtr<UTMS_GameSettings> PendingSettings;
	
	int32 CurTab = 0;
	
	void InitWidget();
	void BindWidgets();
	void UnbindWidgets();

	void LoadSettingsToUI();
	
	void SaveSettings();
	
	void ApplyPendingSettings();
	void ApplyAudioSettings();

	void RevertPendingSettings();
	
	void UpdateChangesIndicator();
	
	void UpdateSliderLabels();
	
	FString GetQualityText(int32 Quality);
	
	void InitComboBoxes();
};
