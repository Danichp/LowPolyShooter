// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEquipmentObject.h"
#include "TMS_LS/Core/Data/TMS_DataTypes.h"
#include "ItemWeaponObject.generated.h"

class UTMS_WeaponData;
class UTMS_StaticWeaponData;

/**
 * 
 */
UCLASS()
class TMS_LS_API UItemWeaponObject : public UItemEquipmentObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TMap<EItemRarity, UTMS_WeaponData*> WeaponDataAssets;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UTMS_StaticWeaponData> StaticWeaponData;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Weapon")
	FWeaponRuntimeStats GeneratedStats;
	
	virtual void InitializeEquipment(EItemRarity InRarity) override;
	
	void SetGeneratedStats(const FWeaponRuntimeStats& Stats);
	
	virtual TSharedPtr<FJsonObject> SerializeToJson() const override;
	virtual bool DeserializeFromJson(const TSharedPtr<FJsonObject>& JsonObject) override;
	
};
