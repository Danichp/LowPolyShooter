// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TMS_LS/Core/Inventory/ItemDataTypes.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TMS_LS_API UItemObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemData ItemData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount = 1;
	
	virtual TSharedPtr<FJsonObject> SerializeToJson() const;
	virtual bool DeserializeFromJson(const TSharedPtr<FJsonObject>& JsonObject);
	
	static const FString JsonKey_ClassName;
	static const FString JsonKey_ItemID;
	static const FString JsonKey_Amount;
	static const FString JsonKey_Rarity;
};
