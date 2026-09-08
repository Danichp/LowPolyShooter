// Fill out your copyright notice in the Description page of Project Settings.

#include "TMS_WeaponComponent.h"

#include "TMS_LS/Core/TMS_Player.h"

UTMS_WeaponComponent::UTMS_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTMS_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// SpawnWeapon();
}

void UTMS_WeaponComponent::SpawnWeapon()
{
	if (!GetWorld() || !GetOwner() || !WeaponClass) return;

	ATMS_BaseCharacter* Player = Cast<ATMS_BaseCharacter>(GetOwner());
	if (!Player) return;
	
	if (ATMS_BaseWeapon* NewWeapon = GetWorld()->SpawnActor<ATMS_BaseWeapon>(WeaponClass))
	{
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentWeapon->WeaponMesh->SetSimulatePhysics(true);
			CurrentWeapon = nullptr;
		}
		
		CurrentWeapon = NewWeapon;
		CurrentWeapon->SetOwner(GetOwner());
		CurrentWeapon->AttachToComponent(Player->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale, CurrentWeapon->SocketName);

		if (CurrentWeapon->AnimLayer)
		{
			Player->GetMesh()->GetAnimInstance()->LinkAnimClassLayers(NewWeapon->AnimLayer);
		}
	}
	OnWeaponUpdate.Broadcast();
	
}

void UTMS_WeaponComponent::UseWeapon(EWeaponActionType Action, bool bInValue)
{
	if (!CurrentWeapon) return;

	switch (Action)
	{
	case EWeaponActionType::EWAT_Main:
		CurrentWeapon->Main_Input(bInValue);
		break;
	case EWeaponActionType::EWAT_Secondary:
		CurrentWeapon->Secondary_Input(bInValue);
		OnAim.Broadcast(bInValue);
		break;
	case EWeaponActionType::EWAT_Reload:
		CurrentWeapon->Reload_Input();
		break;
	case EWeaponActionType::EWAT_MAX:
		break;
	}
}

void UTMS_WeaponComponent::SetCurrentWeapon(ATMS_BaseWeapon* InWeaponActor)
{
	if (!GetWorld()) return;
	ATMS_BaseCharacter* Player = Cast<ATMS_BaseCharacter>(GetOwner());
	if (!Player) return;
	
	if (InWeaponActor)
	{
		CurrentWeapon = InWeaponActor;
		InWeaponActor->SetOwner(GetOwner());

		if (InWeaponActor->AnimLayer)
		{
			Player->GetMesh()->GetAnimInstance()->LinkAnimClassLayers(InWeaponActor->AnimLayer);
		}
	}
	else
	{
		Player->GetMesh()->GetAnimInstance()->LinkAnimClassLayers(Player->BaseAnimLayer);
	}
	OnWeaponUpdate.Broadcast();
}

