// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "ItemEquipmentObject.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UItemEquipmentObject : public UItemObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	EEquipmentType EquipmentType = EEquipmentType::EET_MAX;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	EItemRarity ItemRarity = EItemRarity::EIR_MAX;
	
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	virtual void InitializeEquipment(EItemRarity InRarity);
	
	virtual TSharedPtr<FJsonObject> SerializeToJson() const override;
	virtual bool DeserializeFromJson(const TSharedPtr<FJsonObject>& JsonObject) override;
};
