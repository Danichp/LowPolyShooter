// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TMS_SliderTemplate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSliderValueChangedSignature, const float, NewValue);

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_SliderTemplate : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSliderValueChangedSignature OnSliderValueChanged;
	
	UFUNCTION(BlueprintNativeEvent)
	float GetSliderValue();
	float GetSliderValue_Implementation() {return 0;};
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSliderValue(float NewValue);
	void SetSliderValue_Implementation(float NewValue) {};
};
