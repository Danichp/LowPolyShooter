// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TMS_ClearBlackboardValue.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_ClearBlackboardValue : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTMS_ClearBlackboardValue();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BlackboardKey;
};

