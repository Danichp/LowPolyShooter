// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_FindRandomLocationTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UTMS_FindRandomLocationTask::UTMS_FindRandomLocationTask()
{
	NodeName = "TMS PointSearch";
}

EBTNodeResult::Type UTMS_FindRandomLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Cont = OwnerComp.GetAIOwner();
	const auto Blackboard =  OwnerComp.GetBlackboardComponent();
	if (!Cont || !Blackboard) return EBTNodeResult::Failed;

	const auto Pawn = Cont->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	const auto NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavSystem) return EBTNodeResult::Failed;

	FNavLocation NavRes;

	FVector Location = Pawn->GetActorLocation();

	if (!bSelfCentered)
	{
		if (AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName)))
		{
			Location = Target->GetActorLocation();
		}
	}

	bool Result = NavSystem->GetRandomPointInNavigableRadius(Location, Radius, NavRes);
	if (!Result) return EBTNodeResult::Failed;

	DrawDebugSphere(GetWorld(), NavRes.Location,
		10.f, 16, FColor::Yellow,
		false, 5.f,
		0, 2.f);
	
	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, NavRes.Location);
	return EBTNodeResult::Succeeded;
}
