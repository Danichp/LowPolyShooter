// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TMS_BaseWeapon.h"
#include "NiagaraComponent.h"
#include "TMS_FireWeapon.generated.h"

UCLASS()
class TMS_LS_API ATMS_FireWeapon : public ATMS_BaseWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATMS_FireWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraComponent> FXComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|State")
	bool bIsActive = false;

	bool bReloading = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	int32 MaxAmmo = 30;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	float Speed = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	float AimingFOV = 70.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	float FOV_InterpSpeed = 2.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	float EffectiveDistance = 1500.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	float Distance = 2000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	FVector2D Recoil = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	FVector2D AimRecoil = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	FVector2D AccuracyAngle = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	FVector2D AimAccuracyAngle = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponAnimData ReloadAnims;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentAmmo = 0;

	UPROPERTY(BlueprintAssignable)
	FOnAmmoUpdateSignature OnAmmoUpdate;
		
	FTimerHandle CooldownHandle;
	
	bool bAiming;

protected:
	virtual void BeginPlay() override;

	virtual void Shoot();

	void ApplyRecoil();

	void SetAmmo(int32 InAmmo);

	virtual void Reload();

	FVector GetMuzzleLocation();

	bool GetPlayerViewPoint(FVector& OutViewLocation, FRotator& OutviewDirection);

	UFUNCTION()
	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd);

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnAnimNotify(EWeaponActionType WeaponAction) override;

	virtual void Main_Input(bool bInActive) override;

	virtual void Secondary_Input(bool bInActive) override;

	virtual void Reload_Input() override;
};
