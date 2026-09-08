#pragma once

#include "CoreMinimal.h"
#include "ParticleHelper.h"
#include "TMS_SettingsData.generated.h"

UENUM(BlueprintType)
enum class ESettingsCategory : uint8
{
	ESC_General,
	ESC_Music,
	ESC_Graphics,
	ESC_MAX
};

UENUM(BlueprintType)
enum class ESettingType : uint8
{
	EST_Toggle,
	EST_Slider,
	EST_Switcher,
	EST_Dropdown,
	EST_MAX
};

USTRUCT(BlueprintType, Blueprintable)
struct FSettingData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText SettingName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESettingsCategory SettingCategory = ESettingsCategory::ESC_MAX;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESettingType SettingType = ESettingType::EST_MAX;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "SettingType == ESettingType::EST_Toggle", EditConditionHides))
	bool bDefaultValue = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "SettingType == ESettingType::EST_Slider", EditConditionHides))
	FVector2D SliderRange = FVector2D(0.f, 1.f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "SettingType == ESettingType::EST_Slider", EditConditionHides))
	float DefaultValue = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "SettingType == ESettingType::EST_Switcher || SettingType == ESettingType::EST_Dropdown", EditConditionHides))
	TArray<FName> DefaultValueStrings;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "SettingType == ESettingType::EST_Switcher || SettingType == ESettingType::EST_Dropdown", EditConditionHides))
	bool bAutogen = false;
};