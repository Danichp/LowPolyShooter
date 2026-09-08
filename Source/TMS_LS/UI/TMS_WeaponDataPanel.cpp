// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_WeaponDataPanel.h"

void UTMS_WeaponDataPanel::OnUpdateAmmo(int32 NewAmmo)
{
	FString Ammo = FString::Printf(TEXT("%i"), NewAmmo);
	AmmoText->SetText(FText::FromString(Ammo));
}

void UTMS_WeaponDataPanel::OnUpdateWeapon()
{
	if (!WeaponComponent)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	if (CurWeapon)
	{
		CurWeapon->OnAmmoUpdate.RemoveAll(this);
	}

	CurWeapon = WeaponComponent->GetFireWeapon();
	if (!CurWeapon)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	SetVisibility(ESlateVisibility::Visible);
	
	CurWeapon->OnAmmoUpdate.AddDynamic(this, &UTMS_WeaponDataPanel::OnUpdateAmmo);

	FString Ammo = FString::Printf(TEXT("%i"), CurWeapon->MaxAmmo);
	MaxAmmoText->SetText(FText::FromString(Ammo));
	OnUpdateAmmo(CurWeapon->CurrentAmmo);
}

void UTMS_WeaponDataPanel::NativeConstruct()
{
	Super::NativeConstruct();
	check(AmmoText);
	check(MaxAmmoText);

	if (!GetOwningPlayerPawn()) return;

	if (UTMS_WeaponComponent* WC = GetOwningPlayerPawn()->GetComponentByClass<UTMS_WeaponComponent>())
	{
		WeaponComponent = WC;
		if (WeaponComponent->CurrentWeapon)
		{
			OnUpdateWeapon();
		}
		WeaponComponent->OnWeaponUpdate.AddDynamic(this, &UTMS_WeaponDataPanel::OnUpdateWeapon);
	}
}
