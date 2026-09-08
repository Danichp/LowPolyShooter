// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_BaseCharacter.h"

#include "NavigationInvokerComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TMS_LS/Components/TMS_ArmorComponent.h"
#include "TMS_LS/Components/TMS_WeaponComponent.h"

// Sets default values
ATMS_BaseCharacter::ATMS_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UTMS_HealthComponent>(TEXT("HealthComponent"));

	InventoryComponent = CreateDefaultSubobject<UTMS_InventoryComponent>(TEXT("InventoryComponent"));

	WeaponComponent = CreateDefaultSubobject<UTMS_WeaponComponent>("WeaponComponent");
	
	ArmorComponent = CreateDefaultSubobject<UTMS_ArmorComponent>("ArmorComponent");
	
	EquipmentComponent = CreateDefaultSubobject<UTMS_EquipmentComponent>(TEXT("EquipmentComponent"));

	NavInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));
}

void ATMS_BaseCharacter::OnEquipmentChanged()
{
	if (!ArmorComponent->GetCurrentEquipment() && IsValid(DefaultEquipmentData))
	{
		ArmorComponent->SetEquipment(DefaultEquipmentData);
	}
}

void ATMS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(HealthComponent))
	{
		HealthComponent->OnDeath.AddDynamic(this, &ATMS_BaseCharacter::OnDeath);
	}

	if (IsValid(ArmorComponent))
	{
		ArmorComponent->OnEquipmentChanged.BindUObject(this, &ATMS_BaseCharacter::OnEquipmentChanged);
	}
	OnEquipmentChanged();
}

float ATMS_BaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount < 0)
	{
		HealthComponent->RestoreHealth(-DamageAmount);
		return DamageAmount;
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
}

void ATMS_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATMS_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATMS_BaseCharacter::PlayFootstepSoundBasedOnMaterial(UPhysicalMaterial* InPM, FVector InLocation)
{
	if (!FootstepSounds) return;
	if (!InPM) return;
	
	if (USoundBase* Sound = FootstepSounds->GetPhysicalSoundBasedOnMaterial(InPM))
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, InLocation);
	}
	
	if (UNiagaraSystem* NS = FootstepSounds->GetPhysicalNiagaraBasedOnMaterial(InPM))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NS, InLocation);
	}
}

void ATMS_BaseCharacter::OnDeath()
{
	if (!VaultAnims.IsValid()) return;
	PlayAnimMontage(VaultAnims.DeathMontage);
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->UnPossess();
		PC->StartSpectatingOnly();
		SetLifeSpan(5.f);
		OnEnemyDestroyed.Broadcast();
	}
}
