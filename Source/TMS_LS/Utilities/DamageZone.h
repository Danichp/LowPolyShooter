// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageZone.generated.h"

UCLASS()
class TMS_LS_API ADamageZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADamageZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere)
	float Radius = 300.f;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	UPROPERTY(EditAnywhere)
	FColor Color = FColor::Green;
	
	UPROPERTY(EditAnywhere)
	float DamageDelay = 1.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageType;

	UFUNCTION(BlueprintCallable)
	void DoDamage();
};
