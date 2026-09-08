// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TMS_LS/Core/Data/TMS_DataTypes.h"
#include "TMS_LS/Core/Inventory/Actors/ItemEquipment.h"
#include "TMS_LS/Core/Data/TMS_PhysicalSoundData.h"
#include "TMS_BaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoUpdateSignature, int32, NewAmmo);

UCLASS(BlueprintType, Blueprintable)
class TMS_LS_API ATMS_BaseWeapon : public AItemEquipment
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATMS_BaseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	TSubclassOf<UAnimInstance> AnimLayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Data")
	TObjectPtr<UTMS_PhysicalSoundData> HitReactionData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	float Damage = 10.f;

	virtual void Main_Input(bool bInActive);
	virtual void Secondary_Input(bool bInActive);
	virtual void Reload_Input();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnAnimNotify(EWeaponActionType WeaponAction);
	
	APlayerController* GetPlayerController() const;

	
};
