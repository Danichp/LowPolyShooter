// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_HealthComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TMS_LS/Core/TMS_Player.h"
#include "TMS_LS/Core/Data/TMS_MovementData.h"

UTMS_HealthComponent::UTMS_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UTMS_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);
	SetStamina(MaxStamina);
	Player = Cast<ATMS_Player>(GetOwner());
	if (Player)
	{
		MovementComponent = Player->GetCharacterMovement();
	}

	if (GetOwner())
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UTMS_HealthComponent::OnTakeDamage);
	}

}

void UTMS_HealthComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (bDead || Damage < 0.f) return;


	UE_LOG(LogTemp, Display, TEXT("ResultDamage: %f"), Damage);
	SetHealth(GetHealth() - Damage);

	ATMS_Player* InstigatorPlayer  = nullptr;
	if (InstigatedBy && InstigatedBy->GetPawn())
	{
		InstigatorPlayer = Cast<ATMS_Player>(InstigatedBy->GetPawn());
	}

	if (Health <= 0.f)
	{
		bDead = true;
		if (!InstigatedBy) return;

        if (InstigatorPlayer )
		{
			InstigatorPlayer ->OnKill(DamagedActor);
		}

		OnDeath.Broadcast();
	}
	else
	{
		if (InstigatorPlayer )
		{
			InstigatorPlayer ->OnHit(DamagedActor, Damage);
		}
	}
}


// Called every frame
void UTMS_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bSprinting)
	{
		ReduceStamina(DeltaTime);
	}
	else
	{
		GainStamina(DeltaTime);
	}

	//Fall Damage
	if (bHasFallDamage)
	{
		if (IsValid(MovementComponent) && MovementComponent->IsFalling())
		{
			SetStartFallLocation();
		} else
		{
			CheckFallDamage();
		}
	}

	// ...
}

void UTMS_HealthComponent::SetHealth(float NewValue)
{
	Health = FMath::Clamp(NewValue, 0, MaxHealth);
}

void UTMS_HealthComponent::SetStamina(float NewValue)
{
	Stamina = FMath::Clamp(NewValue, 0, MaxStamina);
}

void UTMS_HealthComponent::RestoreHealth(float InHealth)
{
	SetHealth(GetHealth() + InHealth);
}

void UTMS_HealthComponent::GainStamina(float DeltaTime)
{
	if (!MovementData || bCooldown) return;

	SetStamina(GetStamina() + DeltaTime * MovementData->StaminaGainPerSecond);
}

void UTMS_HealthComponent::ReduceStamina(float DeltaTime)
{
	if (!MovementData) return;
	SetStamina(GetStamina() - DeltaTime * MovementData->StaminaCostPerSecond);

	if (GetStamina() <= 0)
	{
		SprintInput(false);
	}
}

void UTMS_HealthComponent::SprintInput(bool InValue)
{
	if (!GetWorld()) return;
	if (InValue)
	{
		bCooldown = true;
		if (GetWorld()->GetTimerManager().IsTimerActive(CoolDownHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(CoolDownHandle);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(CoolDownHandle,
			this, &UTMS_HealthComponent::FinishCooldown, MovementData->StaminaCooldown);
	}

	if (!CanSprint())
	{
		bSprinting = false;
		return;
	}

	bSprinting = InValue;

	if (!Player) return;
	if (!MovementComponent) return;

	MovementComponent->MaxWalkSpeed = bSprinting ? MovementData->RunSpeed : MovementData->WalkSpeed;
	Player->SetTargetFOV(bSprinting ? MovementData->RunFOV : MovementData->WalkFOV);

}

bool UTMS_HealthComponent::CanSprint()
{
	if (!IsValid(MovementData)) return false;
	return true;
}

void UTMS_HealthComponent::FinishCooldown()
{
	bCooldown = false;
}

void UTMS_HealthComponent::SetStartFallLocation()
{
	if (!Player) return;
	if (StartFallLocation == FVector::ZeroVector)
	{
		StartFallLocation = Player->GetActorLocation();
	}
}

void UTMS_HealthComponent::CheckFallDamage()
{
	if (!IsValid(Player)) return;
	if (StartFallLocation != FVector::ZeroVector)
	{
		FVector FinishFallLocation = Player->GetActorLocation();
		float HeightPlayer = Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*2;
		float HeightFall = FMath::Abs(StartFallLocation.Z - FinishFallLocation.Z);
		if (HeightFall <= HeightPlayer*3)
		{
			StartFallLocation = FVector::ZeroVector;
		} else
		{
			float FallDamage = (HeightFall/HeightPlayer - 3) * FallDamageMultiply;
			StartFallLocation = FVector::ZeroVector;
			OnTakeDamage(
				GetOwner(),
				FallDamage,
				nullptr,
				Player->GetController(),
				nullptr
			);

		}

	}
}
