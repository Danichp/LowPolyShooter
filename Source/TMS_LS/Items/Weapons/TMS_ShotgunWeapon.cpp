// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_ShotgunWeapon.h"

#include "Engine/DamageEvents.h"
#include "TMS_LS/Core/TMS_Player.h"


// Sets default values
ATMS_ShotgunWeapon::ATMS_ShotgunWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATMS_ShotgunWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATMS_ShotgunWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATMS_ShotgunWeapon::Shoot()
{
	if (!bIsActive)
	{
		GetWorldTimerManager().ClearTimer(CooldownHandle);
		return;
	}
	
	if (CurrentAmmo == 0)
	{
		Reload_Input();
		return;
	}
	
	FHitResult Hit;
	
	ATMS_Player* Char = Cast<ATMS_Player>(GetOwner());
	if (!Char) return;
	
	FVector StartLocation;
	FVector EndLocation;
	
	
	SetAmmo(CurrentAmmo-1);
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Char);
	IgnoreActors.Add(this);
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(IgnoreActors);
	
	
	for (int i = 0; i < CountFraction; ++i)
	{
		
		if (!GetTraceData(StartLocation, EndLocation))
		{
			continue;
		}
		
		GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation,
	ECC_Camera, CollisionParams);
	
		if (Hit.bBlockingHit)
		{
			EndLocation = Hit.Location;
		}
		
		GetWorld()->LineTraceSingleByChannel(Hit,
	GetMuzzleLocation(), EndLocation,
	ECollisionChannel::ECC_Pawn, CollisionParams);
		DrawDebugLine(GetWorld(),GetMuzzleLocation(), EndLocation, FColor::Green, false, 0.5f, 0, 1.f);
	
		if (Hit.bBlockingHit && Hit.GetActor())
		{
			Hit.GetActor()->TakeDamage(Damage, FDamageEvent{}, Char->GetController(), Char);
		}
		
	}
	
	ApplyRecoil();
	
	GetWorldTimerManager().SetTimer(CooldownHandle,
			this, &ThisClass::Shoot, 1/Speed);
	
}

