// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TMS_LS/Components/TMS_WeaponComponent.h"
#include "TMS_WeaponDataPanel.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_WeaponDataPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxAmmoText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTMS_WeaponComponent> WeaponComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ATMS_FireWeapon> CurWeapon;

	UFUNCTION()
	void OnUpdateAmmo(int32 NewAmmo);

	UFUNCTION()
	void OnUpdateWeapon();

	virtual void NativeConstruct() override;
	
};
