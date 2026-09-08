// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TMS_HealthCheckService.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_HealthCheckService : public UBTService
{
	GENERATED_BODY()
public:
	UTMS_HealthCheckService();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Check")
	FBlackboardKeySelector DangerStateKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Check", meta = (Units = "Percent"))
	float SafeHealthPercentage = 50.f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
