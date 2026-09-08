// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"

const FString UItemObject::JsonKey_ClassName = TEXT("ClassName");
const FString UItemObject::JsonKey_ItemID = TEXT("ItemID");
const FString UItemObject::JsonKey_Amount = TEXT("Amount");
const FString UItemObject::JsonKey_Rarity = TEXT("Rarity");

TSharedPtr<FJsonObject> UItemObject::SerializeToJson() const
{
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject);
	JsonObj->SetStringField(JsonKey_ClassName, GetClass()->GetPathName());
	JsonObj->SetStringField(JsonKey_ItemID, ItemData.ItemID.ToString());
	JsonObj->SetNumberField(JsonKey_Amount, Amount);
	return JsonObj;
}

bool UItemObject::DeserializeFromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!JsonObject.IsValid()) return false;
	
	FString ItemIDString;
	if (!JsonObject->TryGetStringField(JsonKey_ItemID, ItemIDString))
	{
		return false;
	}
	ItemData.ItemID = FName(*ItemIDString);
	
	int32 AmountValue;
	if (!JsonObject->TryGetNumberField(JsonKey_Amount, AmountValue))
	{
		return false;
	}
	Amount = AmountValue;
	
	return true;
}
