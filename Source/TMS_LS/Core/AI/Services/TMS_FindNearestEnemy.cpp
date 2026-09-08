// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_FindNearestEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TMS_LS/Core/AI/TMS_AIPerception.h"

UTMS_FindNearestEnemy::UTMS_FindNearestEnemy()
{
	NodeName = "TMS Find Nearest Enemy";
}

void UTMS_FindNearestEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		const auto Cont = OwnerComp.GetAIOwner();
		if (Cont)
		{
			const auto Perception = Cont->GetComponentByClass<UTMS_AIPerception>();
			if (Perception)
			{
				Blackboard->SetValueAsObject(TargetEnemyKey.SelectedKeyName,
					Perception->GetClosestEnemy().Get());
			}
		}
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
