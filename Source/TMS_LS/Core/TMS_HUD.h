// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "TMS_HUD.generated.h"


UENUM(BlueprintType)
enum class EUIState : uint8
{
	EUIS_Game,
	EUIS_Pause,
	EUIS_Loot,
	EUIS_Score,
	EUIS_Equipment
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIStateChangedSignature, EUIState, NewState);

/**
 * 
 */
UCLASS()
class TMS_LS_API ATMS_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

	virtual void PostRender() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EUIState GetUIState() const { return CurrentUIState; };

	UFUNCTION(BlueprintCallable)
	void SetUIState(EUIState InState);

	UPROPERTY(BlueprintAssignable)
	FOnUIStateChangedSignature OnUIStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> HUD_Widget;

private:
	void DrawCrosshair();
	
	UPROPERTY()
	TArray<UUserWidget*> HUD_Widgets;

	UPROPERTY()
	EUIState CurrentUIState;
};
