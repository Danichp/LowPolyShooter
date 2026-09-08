// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_InventorySubsystem.h"
#include "TMS_InventoryComponent.h"
#include "TMS_LS/Utilities/TMS_DeveloperSettings.h"

bool UTMS_InventorySubsystem::AddItemToController(APlayerController* Target, UItemObject* Item)
{
	if (!Target || !Target->GetPawn()) return false;
	UTMS_InventoryComponent* IC = Target->GetPawn()->GetComponentByClass<UTMS_InventoryComponent>();
	if (!IC) return false;
	IC->AddItemAsObject(Item);
	return true;
}

bool UTMS_InventorySubsystem::AddItem(AActor* Target,  UItemObject* Item)
{
	if (!Target) return false;
	UTMS_InventoryComponent* IC = Target->GetComponentByClass<UTMS_InventoryComponent>();
	if (!IC) return false;
	bool Result = false;
	IC->AddItemAsObject(Item);
	return true;
}

void UTMS_InventorySubsystem::GetItemData(FName ItemID, FItemData& OutItem)
{
	const UTMS_DeveloperSettings* Settings = UTMS_DeveloperSettings::Get();
	if (!IsValid(Settings) || !IsValid(Settings->ItemDataTable.LoadSynchronous())) return;

	OutItem = *Settings->ItemDataTable->FindRow<FItemData>(ItemID, "");
}
