// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TMS_AttributePanel.h"
#include "Blueprint/UserWidget.h"
#include "TMS_LS/Core/TMS_HUD.h"
#include "TMS_Main.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_Main : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTMS_AttributePanel> AttributePanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD_Setup")
	TSubclassOf<class UTMS_Pause> PauseWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD_Setup")
	TObjectPtr<UTMS_Pause> PauseWidget;

	UFUNCTION(BlueprintNativeEvent)
	void OnKilled(AActor* KilledActor);
	virtual void OnKilled_Implementation(AActor* KilledActor);

	UFUNCTION(BlueprintNativeEvent)
	void OnDamaged(AActor* DamagedActor, float Damage);
	virtual void OnDamaged_Implementation(AActor* DamagedActor, float Damage);
	
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUIStateChanged(EUIState InNewState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUIState CurrentUIState;

	
	
};
