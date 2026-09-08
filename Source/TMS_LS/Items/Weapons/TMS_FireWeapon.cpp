// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_FireWeapon.h"

#include "MeshPaintVisualize.h"
#include "Engine/DamageEvents.h"
#include "TMS_LS/Core/TMS_BaseCharacter.h"

// Sets default values
ATMS_FireWeapon::ATMS_FireWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FXComponent"));
	FXComponent->SetupAttachment(WeaponMesh);
}

// Called when the game starts or when spawned
void ATMS_FireWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmo = MaxAmmo;
}

// Called every frame
void ATMS_FireWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATMS_FireWeapon::OnAnimNotify(EWeaponActionType WeaponAction)
{
	switch (WeaponAction)
	{
	case EWeaponActionType::EWAT_Main:
		break;
	case EWeaponActionType::EWAT_Secondary:
		break;
	case EWeaponActionType::EWAT_Reload:
		bReloading = false;
		SetAmmo(MaxAmmo);
		break;
	case EWeaponActionType::EWAT_MAX:
		break;
	}
}

void ATMS_FireWeapon::Main_Input(bool bInActive)
{
	if (bReloading) return;
	if (bIsActive != bInActive)
	{
		if (bInActive)
		{
			FXComponent->Activate(true);
		}
		else
		{
			FXComponent->Deactivate();
		}
	}
	bIsActive = bInActive;
	if (bInActive && !GetWorldTimerManager().IsTimerActive(CooldownHandle))
	{
		Shoot();
	}
	Super::Main_Input(bInActive);
}

void ATMS_FireWeapon::Secondary_Input(bool bInActive)
{
	bAiming = bInActive;
	Super::Secondary_Input(bInActive);
}

void ATMS_FireWeapon::Reload_Input()
{
	Main_Input(false);
	Secondary_Input(false);
	Reload();
	Super::Reload_Input();
}

void ATMS_FireWeapon::Shoot()
{
	if (!bIsActive || bReloading)
	{
		GetWorldTimerManager().ClearTimer(CooldownHandle);
		return;
	}

	if (CurrentAmmo == 0)
	{
		if (!bReloading)
		{
			Reload_Input();
		}
		return;
	}
	
	FHitResult Hit;

	ATMS_BaseCharacter* Char = Cast<ATMS_BaseCharacter>(GetOwner());
	if (!Char) return;

	FVector StartLocation;
	FVector EndLocation;

	if (!GetTraceData(StartLocation, EndLocation)) return;

	SetAmmo(CurrentAmmo-1);
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Char);
	IgnoreActors.Add(this);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(IgnoreActors);

	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation,
		ECC_GameTraceChannel4, CollisionParams);

	if (Hit.bBlockingHit)
	{
		FVector Direction = (Hit.Location - GetMuzzleLocation()).GetSafeNormal();
		EndLocation = GetMuzzleLocation() + Direction * Distance;
	}

	GetWorld()->LineTraceSingleByChannel(Hit,
		GetMuzzleLocation(), EndLocation,
		ECollisionChannel::ECC_GameTraceChannel4, CollisionParams);
	DrawDebugLine(GetWorld(),GetMuzzleLocation(), EndLocation, FColor::Green, false, 2.5f, 0, 1.f);
	
#if !UE_BUILD_SHIPPING
	if (CVarDebugMode.GetValueOnGameThread())
	{
		DrawDebugLine(GetWorld(),GetMuzzleLocation(), EndLocation, FColor::Green, false, 0.5f, 0, 1.f);
	}
#endif
	
	if (Hit.bBlockingHit && Hit.GetActor())
	{
		Hit.GetActor()->TakeDamage(Damage, FDamageEvent{}, Char->GetController(), Char);
	}
	
	ApplyRecoil();

	GetWorldTimerManager().SetTimer(CooldownHandle,
			this, &ThisClass::Shoot, 1/Speed);
	
}

void ATMS_FireWeapon::ApplyRecoil()
{
	FVector2D TarRec = bAiming ? AimRecoil : Recoil;
	float RandX = FMath::DegreesToRadians(FMath::FRandRange(-TarRec.X, TarRec.X));
	float RandY = FMath::DegreesToRadians(FMath::FRandRange(-TarRec.Y, TarRec.Y));
	ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (!Player) return;
	Player->AddControllerYawInput(RandX);
	Player->AddControllerPitchInput(RandY);
}

void ATMS_FireWeapon::SetAmmo(int32 InAmmo)
{
	CurrentAmmo = FMath::Clamp(InAmmo, 0, MaxAmmo);
	OnAmmoUpdate.Broadcast(CurrentAmmo);
}

void ATMS_FireWeapon::Reload()
{
	if (CurrentAmmo == MaxAmmo) return;

	bReloading = true;
	bIsActive = false;
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return;

	if (!ReloadAnims.CAnim) return;
	float Length = Player->PlayAnimMontage(ReloadAnims.CAnim);	
	
	if (UAnimInstance* WeaponAnim = WeaponMesh->GetAnimInstance())
	{
		WeaponAnim->Montage_Play(ReloadAnims.WAnim);
	}
	else
	{
		FTimerDelegate ReloadDelegate;
		ReloadDelegate.BindUObject(this, &ThisClass::OnAnimNotify, EWeaponActionType::EWAT_Reload);
		FTimerHandle ReloadHandle;
		GetWorldTimerManager().SetTimer(ReloadHandle,ReloadDelegate,
			Length * 0.8, false);
	}
	
}

FVector ATMS_FireWeapon::GetMuzzleLocation()
{
	return WeaponMesh->GetSocketLocation(FName("S_Muzzle"));
}

bool ATMS_FireWeapon::GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutviewDirection)
{
	const ATMS_BaseCharacter* Player = Cast<ATMS_BaseCharacter>(GetOwner());
	if (!Player) return false;
	
	if (Player->IsPlayerControlled())
	{
		const APlayerController* PC = GetPlayerController();
		if (!PC) return false;

		PC->GetPlayerViewPoint(OutViewLocation, OutviewDirection);
	}
	else
	{
		OutViewLocation = GetMuzzleLocation();
		OutviewDirection = WeaponMesh->GetSocketRotation(FName("S_Muzzle"));
	}
	return true;
}

bool ATMS_FireWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FRotator ViewDirection;
	FVector ViewLocation;

	if (!GetPlayerViewPoint(ViewLocation, ViewDirection)) return false;

	TraceStart = ViewLocation;

	float RadX = FMath::DegreesToRadians(bAiming ? AimAccuracyAngle.X : AccuracyAngle.X);
	float RadY = FMath::DegreesToRadians(bAiming ? AimAccuracyAngle.Y : AccuracyAngle.Y);
	
	FVector TraceDirection = FMath::VRandCone(ViewDirection.Vector(),RadX/2, RadY/2);
	TraceEnd = TraceStart + TraceDirection * Distance;
	return true;
}