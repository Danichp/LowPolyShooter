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
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;

	AAIController* Cont = OwnerComp.GetAIOwner();
	if (!Cont) return;

	UTMS_AIPerception* Perception = Cont->GetComponentByClass<UTMS_AIPerception>();
	if (!Perception) return;

	TWeakObjectPtr<AActor> ClosestEnemy = Perception->GetClosestEnemy();

	if (ClosestEnemy.IsValid())
	{
		AActor* Enemy = ClosestEnemy.Get();
		Blackboard->SetValueAsObject(TargetEnemyKey.SelectedKeyName, Enemy);
		Blackboard->SetValueAsVector(LastKnownLocationKey.SelectedKeyName, Enemy->GetActorLocation());
	}
	else
	{
		Blackboard->SetValueAsObject(TargetEnemyKey.SelectedKeyName, nullptr);
	}
}