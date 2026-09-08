// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TMS_AttackService.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_AttackService : public UBTService
{
	GENERATED_BODY()
public:
	UTMS_AttackService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
