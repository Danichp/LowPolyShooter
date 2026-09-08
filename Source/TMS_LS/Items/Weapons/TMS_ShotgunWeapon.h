// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TMS_BaseWeapon.h"
#include "TMS_FireWeapon.h"
#include "TMS_ShotgunWeapon.generated.h"

UCLASS()
class TMS_LS_API ATMS_ShotgunWeapon : public ATMS_FireWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATMS_ShotgunWeapon();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	int32 CountFraction = 8;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Data")
	float SpreadAngle  = 10.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Shoot() override;
};
