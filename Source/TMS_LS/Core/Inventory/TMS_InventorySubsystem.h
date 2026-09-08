// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TMS_InventorySubsystem.generated.h"

class UItemObject;
/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_InventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	bool AddItemToController(APlayerController* Target, UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	bool AddItem(AActor* Target, UItemObject* Item);
		
	UFUNCTION(BlueprintCallable)
	void GetItemData(FName ItemID, FItemData& OutItem);
};
