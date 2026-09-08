// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEquipmentObject.h"

void UItemEquipmentObject::InitializeEquipment(EItemRarity InRarity)
{
	ItemRarity = InRarity;
}

TSharedPtr<FJsonObject> UItemEquipmentObject::SerializeToJson() const
{
	TSharedPtr<FJsonObject> JsonObj = Super::SerializeToJson();
	if (!JsonObj.IsValid()) return nullptr;
	
	JsonObj->SetNumberField(UItemObject::JsonKey_Rarity, static_cast<int32>(ItemRarity));
	return JsonObj;
}

bool UItemEquipmentObject::DeserializeFromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!Super::DeserializeFromJson(JsonObject)) return false;
	
	int32 RarityValue;
	if (!JsonObject->TryGetNumberField(UItemObject::JsonKey_Rarity, RarityValue))
	{
		return false;
	}
	ItemRarity = static_cast<EItemRarity>(RarityValue);
	return true;
	
}
