// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TMS_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

class UTMS_MovementData;
class ATMS_Player;
class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TMS_LS_API UTMS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTMS_HealthComponent();

public:
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	TObjectPtr<UTMS_MovementData> MovementData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attribute)
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attribute)
	float MaxHealth;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attribute)
	float Stamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attribute)
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attribute)
	TMap<TSubclassOf<UDamageType>, float> DamageReductors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bSprinting = false;

	bool bCooldown = false;

	float UpdateFrequency = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bDead = false;

	FTimerHandle CoolDownHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attribute)
	bool bHasFallDamage = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attribute)
	float FallDamageMultiply = 20.f;
	
	UPROPERTY()
	ATMS_Player* Player;
	
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	
	
	FVector StartFallLocation = FVector::ZeroVector;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage,
		const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetHealth() { return Health; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetMaxHealth() { return MaxHealth; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetHealthPercentage() { return Health / MaxHealth; };

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewValue);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetStamina() { return Stamina; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetMaxStamina() { return MaxStamina; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetStaminaPercentage() { return Stamina / MaxStamina; };
	
	UFUNCTION(BlueprintCallable)
	void SetStamina(float NewValue);

	UFUNCTION(BlueprintCallable)
	void RestoreHealth(float InHealth);
	
	UFUNCTION()
	void SprintInput(bool InValue);
private:
	void GainStamina(float DeltaTime);

	void ReduceStamina(float DeltaTime);
	
	bool CanSprint();
	void FinishCooldown();
	
	void SetStartFallLocation();
	
	void CheckFallDamage();
	
};
