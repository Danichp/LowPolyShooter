// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TMS_ButtonTemplate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickedSignature);

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_ButtonTemplate : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnButtonClickedSignature OnButtonClicked;
};
