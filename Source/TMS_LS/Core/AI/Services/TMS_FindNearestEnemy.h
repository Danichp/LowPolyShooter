// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TMS_FindNearestEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_FindNearestEnemy : public UBTService
{
	GENERATED_BODY()

public:
	UTMS_FindNearestEnemy();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
	FBlackboardKeySelector TargetEnemyKey;
};
