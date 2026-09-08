// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TMS_FindRandomLocationTask.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_FindRandomLocationTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTMS_FindRandomLocationTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Radius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data",
		meta = (EditCondition = "bSelfCentered", EditConditionHides))
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bSelfCentered = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data",
		meta = (EditCondition = "!bSelfCentered", EditConditionHides))
	FBlackboardKeySelector TargetActorKey;
	
};
