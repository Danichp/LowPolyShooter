// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_MeleeWeapon.h"

#include "Components/CapsuleComponent.h"
#include "TMS_LS/Core/TMS_BaseCharacter.h"
#include "Engine/DamageEvents.h"


ATMS_MeleeWeapon::ATMS_MeleeWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetupAttachment(WeaponMesh);
	Capsule->SetGenerateOverlapEvents(false);
}

void ATMS_MeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(Capsule))
	{
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &ATMS_MeleeWeapon::OnWeaponCollide);
		Capsule->SetGenerateOverlapEvents(false);
	}
}

void ATMS_MeleeWeapon::OnWeaponCollide(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsAttacking) return;

	ATMS_BaseCharacter* Char = Cast<ATMS_BaseCharacter>(GetOwner());
	if (!Char) return;

	if (OtherActor && OtherActor != Char)
	{
		OtherActor->TakeDamage(Damage, FDamageEvent{}, Char->GetController(), Char);
	}
}

void ATMS_MeleeWeapon::ToggleAttackState()
{
	SetAttackState(!bIsAttacking);
}

void ATMS_MeleeWeapon::SetAttackState(bool bInAttack)
{
	bIsAttacking = bInAttack;
	if (IsValid(Capsule))
	{
		Capsule->SetGenerateOverlapEvents(bIsAttacking);
	}
}

void ATMS_MeleeWeapon::Main_Input(bool bInActive)
{
	Super::Main_Input(bInActive);
	if (AttackAnims.Num() == 0) return;

	ATMS_BaseCharacter* Char = Cast<ATMS_BaseCharacter>(GetOwner());
	if (!Char && Char->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) return;

	int32 IdAtt = FMath::RandRange(0, AttackAnims.Num() - 1);

	float Length = 0.8;

	if (UAnimMontage* CM = AttackAnims[IdAtt].CAnim)
	{
		Length = Char->PlayAnimMontage(CM);
	}
	
	if (UAnimMontage* WM = AttackAnims[IdAtt].WAnim)
	{
		if (WeaponMesh->GetAnimInstance())
		{
			WeaponMesh->GetAnimInstance()->Montage_Play(WM);
		}
	}
	else
	{
		SetAttackState(false);
		FTimerHandle InAttackHandle;
		FTimerHandle OutAttackHandle;

		// Для примера работы таймеров для функций с параметрами
		FTimerDelegate InAttackDelegate;
		InAttackDelegate.BindUObject(this, &ATMS_MeleeWeapon::SetAttackState, true);
		FTimerDelegate OutAttackDelegate;
		OutAttackDelegate.BindUObject(this, &ATMS_MeleeWeapon::SetAttackState, false);

		GetWorldTimerManager().SetTimer(InAttackHandle, InAttackDelegate,
			Length * 0.1f, false);

		GetWorldTimerManager().SetTimer(OutAttackHandle, OutAttackDelegate,
			Length*0.8f, false);
	}
}

void ATMS_MeleeWeapon::Secondary_Input(bool bInActive)
{
	Super::Secondary_Input(bInActive);
}

void ATMS_MeleeWeapon::OnAnimNotify(EWeaponActionType WeaponAction)
{
	switch (WeaponAction)
	{
	case EWeaponActionType::EWAT_Main:
		ToggleAttackState();
		break;
	case EWeaponActionType::EWAT_Secondary:
		break;
	case EWeaponActionType::EWAT_Reload:
		break;
	case EWeaponActionType::EWAT_MAX:
		break;
	}
	Super::OnAnimNotify(WeaponAction);
}

