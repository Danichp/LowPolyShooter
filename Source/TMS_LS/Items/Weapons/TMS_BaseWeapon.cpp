// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"


// Sets default values
ATMS_BaseWeapon::ATMS_BaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	EquipmentType = EEquipmentType::EET_Main;
}

void ATMS_BaseWeapon::Main_Input(bool bInActive)
{
} 

void ATMS_BaseWeapon::Secondary_Input(bool bInActive)
{
}

void ATMS_BaseWeapon::Reload_Input()
{
}

// Called when the game starts or when spawned
void ATMS_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ATMS_BaseWeapon::OnAnimNotify(EWeaponActionType WeaponAction)
{
}

APlayerController* ATMS_BaseWeapon::GetPlayerController() const
{
	const ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
}