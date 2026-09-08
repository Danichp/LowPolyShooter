// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TMS_BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Data/TMS_InputData.h"
#include "GameFramework/SpringArmComponent.h"
#include "MotionWarpingComponent.h"
#include "TMS_LS/TMS_LSCharacter.h"
#include "TMS_HUD.h"
#include "TMS_Player.generated.h"

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> CVarPlayerGodMode(
	TEXT("tms_cheats.Player.GodMode"), 0, TEXT("0 for false, >0 for true"));
#endif

enum class EEquipmentLayer : uint8;
class UTMS_WeaponComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilledSignature, AActor*, KilledActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDealtDamageSignature, AActor*, DamagedActor, float, Damage);
/**
 * 
 */
UCLASS()
class TMS_LS_API ATMS_Player : public ATMS_BaseCharacter
{
	GENERATED_BODY()

	ATMS_Player();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Inputs)
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	TObjectPtr<UMotionWarpingComponent> MotionWarper;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Inputs)
	TObjectPtr<UTMS_InputData> InputData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> InteractActor;
	
	UPROPERTY(BlueprintAssignable)
	FOnKilledSignature OnKilled;
	
	UPROPERTY(BlueprintAssignable)
	FOnDealtDamageSignature OnDamaged;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Skeleton)
	TObjectPtr<USkeletalMeshComponent> HelmetMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Skeleton)
	TObjectPtr<USkeletalMeshComponent> BodyMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Skeleton)
	TObjectPtr<USkeletalMeshComponent> HandsMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Skeleton)
	TObjectPtr<USkeletalMeshComponent> LegsMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Skeleton)
	TObjectPtr<USkeletalMeshComponent> BootsMesh;

private:
	FVector2D MoveInput;

	float TargetFOV = 90.f;
	
	float DefaultJumpBufferTime = 0.15f;
	float CurrentJumpBufferTime = 0.0f;
	float CurrentFallBufferTime = 0.0f;

	float CurrentFOV = 90.f;

	UPROPERTY()
	TObjectPtr<APlayerController> PPC;

	UPROPERTY()
	TObjectPtr<ATMS_HUD> PHUD;
	
protected:
	UFUNCTION()
	void OnInventoryUpdated();
	
	UFUNCTION()
	void OnEquipmentUpdated();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnEquipmentChanged() override;
	
	USkeletalMeshComponent* GetSkMByEquipmentLayer(EEquipmentLayer Layer);

public:
	void MathTick(float DeltaTime);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	void MainInput(const FInputActionValue& InputActionValue);
	void SecondaryInput(const FInputActionValue& InputActionValue);
	void ReloadInput(const FInputActionValue& InputActionValue);

	void CheckInteractable();
	
	void OnKill(AActor* DamagedActor);

	void OnHit(AActor* DamagedActor, float Damage);

	UFUNCTION()
	void OnAimUpdate(bool bNewActive);
	void FOV_Update(float DeltaTime);
	void SetTargetFOV(float NewTarget);

	void OnPauseInput();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool CanJumpInternal_Implementation() const override;
	
	void OnMoveInput(const FInputActionValue& Value);
	void StopMoveInput(const FInputActionValue& Value);
	void OnLookInput(const FInputActionValue& Value);
	void OnSprintInput(const FInputActionValue& Value);
	void OnCrouchInput(const FInputActionValue& Value);
	void OnInventoryInput(const FInputActionValue& Value);
	void OnStatsInput(const FInputActionValue& Value);
	void OnInteractInput(const FInputActionValue& Value);
	
	virtual void Jump() override;

	bool CanVault() const;

	bool TryVault();

	void FinishVault();
};

