// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TMS_LS/Core/Data/TMS_DataTypes.h"
#include "TMS_LS/Items/Weapons/TMS_BaseWeapon.h"
#include "TMS_LS/Items/Weapons/TMS_FireWeapon.h"
#include "TMS_WeaponComponent.generated.h"

class ATMS_FireWeapon;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimSignature, bool, bIsAiming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUpdateSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TMS_LS_API UTMS_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTMS_WeaponComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATMS_BaseWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ATMS_BaseWeapon> CurrentWeapon;

	UPROPERTY(BlueprintAssignable)
	FOnAimSignature OnAim;
	UPROPERTY(BlueprintAssignable)
	FOnWeaponUpdateSignature OnWeaponUpdate;

protected:
	virtual void BeginPlay() override;

public:

	void SpawnWeapon();

	void UseWeapon(EWeaponActionType Action, bool bInValue = true);
	void SetCurrentWeapon(ATMS_BaseWeapon* InWeaponActor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ATMS_FireWeapon* GetFireWeapon() { return Cast<ATMS_FireWeapon>(CurrentWeapon); };
};
