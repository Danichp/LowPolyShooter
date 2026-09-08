// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "TMS_AttributePanel.generated.h"

class UTMS_HealthComponent;
/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_AttributePanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> BarBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTMS_HealthComponent> HealthComponent;
};
