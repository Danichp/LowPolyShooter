// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/TMS_DataTypes.h"
#include "Data/TMS_PhysicalSoundData.h"
#include "GameFramework/Character.h"
#include "Inventory/TMS_EquipmentComponent.h"
#include "TMS_LS/Components/TMS_HealthComponent.h"
#include "Inventory/TMS_InventoryComponent.h"
#include "NavigationInvokerComponent.h"
#include "TMS_BaseCharacter.generated.h"

class UTMS_ArmorComponent;
class UEquipmentConstuctor;
class UNavigationInvokerComponent;
class UTMS_WeaponComponent;
class UTMS_PhysicalSoundData;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDestroyed);

UCLASS()
class TMS_LS_API ATMS_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATMS_BaseCharacter();
	
protected:
	virtual void OnEquipmentChanged();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDestroyed OnEnemyDestroyed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSprinting = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharSetup")
	TSubclassOf<UAnimInstance> BaseAnimLayer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharSetup")
	TObjectPtr<UEquipmentConstuctor> DefaultEquipmentData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	FTMS_AnimData VaultAnims;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	TObjectPtr<UTMS_PhysicalSoundData> FootstepSounds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsVaulting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	TObjectPtr<UTMS_HealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<UTMS_InventoryComponent> InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<UTMS_EquipmentComponent> EquipmentComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	TObjectPtr<UTMS_ArmorComponent> ArmorComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	TObjectPtr<UTMS_WeaponComponent> WeaponComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	TObjectPtr<UNavigationInvokerComponent> NavInvoker;
	
	UFUNCTION(BlueprintCallable)
	void PlayFootstepSoundBasedOnMaterial(UPhysicalMaterial* InPM, FVector InLocation);
	
	
	UFUNCTION()
	virtual void OnDeath();
};
