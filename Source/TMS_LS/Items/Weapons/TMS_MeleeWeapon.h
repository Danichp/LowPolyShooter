// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TMS_BaseWeapon.h"
#include "TMS_MeleeWeapon.generated.h"

class UCapsuleComponent;

UCLASS()
class TMS_LS_API ATMS_MeleeWeapon : public ATMS_BaseWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATMS_MeleeWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCapsuleComponent> Capsule;

	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TArray<FWeaponAnimData> AttackAnims;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnWeaponCollide(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void ToggleAttackState();
	UFUNCTION()
	void SetAttackState(bool bInAttack);

public:

	virtual void Main_Input(bool bInActive) override;
	virtual void Secondary_Input(bool bInActive) override;

	virtual void OnAnimNotify(EWeaponActionType WeaponAction) override;

	
};
