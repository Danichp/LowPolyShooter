// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TMS_CheckboxTemplate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckboxStateChangedSignature, bool, NewValue);

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_CheckboxTemplate : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnCheckboxStateChangedSignature OnCheckboxStateChanged;
	
	UFUNCTION(BlueprintNativeEvent)
	bool GetCheckboxValue();
	bool GetCheckboxValue_Implementation() {return false;};
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCheckboxValue(bool NewValue);
	void SetCheckboxValue_Implementation(bool NewValue) {};
	
};
