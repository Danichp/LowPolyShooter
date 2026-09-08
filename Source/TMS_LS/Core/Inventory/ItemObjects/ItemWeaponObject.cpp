// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWeaponObject.h"

#include "TMS_LS/Core/Data/TMS_WeaponData.h"

void UItemWeaponObject::InitializeEquipment(EItemRarity InRarity)
{
	Super::InitializeEquipment(InRarity);
	
	if (WeaponDataAssets.Contains(InRarity) && WeaponDataAssets[InRarity])
	{
		GeneratedStats = WeaponDataAssets[InRarity]->GenerateStats();
		return;
	}
	
	for (const auto& Pair : WeaponDataAssets)
	{
		if (Pair.Value)
		{
			GeneratedStats = Pair.Value->GenerateStats();
			return;
		}
	}
}

void UItemWeaponObject::SetGeneratedStats(const FWeaponRuntimeStats& Stats)
{
	GeneratedStats = Stats;
}

TSharedPtr<FJsonObject> UItemWeaponObject::SerializeToJson() const
{
	TSharedPtr<FJsonObject> JsonObj = Super::SerializeToJson();
	if (!JsonObj.IsValid()) return nullptr;
	
	TSharedPtr<FJsonObject> StatsObj = MakeShareable(new FJsonObject);
	StatsObj->SetNumberField(TEXT("Damage"), GeneratedStats.Damage);
	StatsObj->SetNumberField(TEXT("FireRate"), GeneratedStats.FireRate);
	StatsObj->SetNumberField(TEXT("MaxAmmo"), GeneratedStats.MaxAmmo);
	StatsObj->SetNumberField(TEXT("RecoilX"), GeneratedStats.Recoil.X);
	StatsObj->SetNumberField(TEXT("RecoilY"), GeneratedStats.Recoil.Y);
	StatsObj->SetNumberField(TEXT("EffectiveRange"), GeneratedStats.EffectiveRange);
	StatsObj->SetNumberField(TEXT("AccuracyAngleX"), GeneratedStats.AccuracyAngle.X);
	StatsObj->SetNumberField(TEXT("AccuracyAngleY"), GeneratedStats.AccuracyAngle.Y);
	StatsObj->SetNumberField(TEXT("AccuracyAimAngleX"), GeneratedStats.AccuracyAimAngle.X);
	StatsObj->SetNumberField(TEXT("AccuracyAimAngleY"), GeneratedStats.AccuracyAimAngle.Y);
	StatsObj->SetNumberField(TEXT("AimFOV"), GeneratedStats.AimFOV);
	StatsObj->SetNumberField(TEXT("AimSpeed"), GeneratedStats.AimSpeed);
	
	JsonObj->SetObjectField(TEXT("WeaponStats"), StatsObj);
	
	return JsonObj;
}

bool UItemWeaponObject::DeserializeFromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!Super::DeserializeFromJson(JsonObject)) return false;
	
	const TSharedPtr<FJsonObject>* StatsObj;
	if (!JsonObject->TryGetObjectField(TEXT("WeaponStats"), StatsObj)) return false;
	
	FWeaponRuntimeStats Stats;
	(*StatsObj)->TryGetNumberField(TEXT("Damage"), Stats.Damage);
	(*StatsObj)->TryGetNumberField(TEXT("FireRate"), Stats.FireRate);
	(*StatsObj)->TryGetNumberField(TEXT("MaxAmmo"), Stats.MaxAmmo);
	(*StatsObj)->TryGetNumberField(TEXT("RecoilX"), Stats.Recoil.X);
	(*StatsObj)->TryGetNumberField(TEXT("RecoilY"), Stats.Recoil.Y);
	(*StatsObj)->TryGetNumberField(TEXT("EffectiveRange"), Stats.EffectiveRange);
	(*StatsObj)->TryGetNumberField(TEXT("AccuracyAngleX"), Stats.AccuracyAngle.X);
	(*StatsObj)->TryGetNumberField(TEXT("AccuracyAngleY"), Stats.AccuracyAngle.Y);
	(*StatsObj)->TryGetNumberField(TEXT("AccuracyAimAngleX"), Stats.AccuracyAimAngle.X);
	(*StatsObj)->TryGetNumberField(TEXT("AccuracyAimAngleY"), Stats.AccuracyAimAngle.Y);
	(*StatsObj)->TryGetNumberField(TEXT("AimFOV"), Stats.AimFOV);
	(*StatsObj)->TryGetNumberField(TEXT("AimSpeed"), Stats.AimSpeed);
	
	SetGeneratedStats(Stats);
	return false;
}
