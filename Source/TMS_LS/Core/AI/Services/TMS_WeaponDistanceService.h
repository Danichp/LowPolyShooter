// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TMS_WeaponDistanceService.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_WeaponDistanceService : public UBTService
{
	GENERATED_BODY()
public:
	UTMS_WeaponDistanceService();

	UPROPERTY(EditAnywhere, Category = Behavior)
	FBlackboardKeySelector MinDistanceKey;

	UPROPERTY(EditAnywhere, Category = Behavior)
	FBlackboardKeySelector MaxDistanceKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
