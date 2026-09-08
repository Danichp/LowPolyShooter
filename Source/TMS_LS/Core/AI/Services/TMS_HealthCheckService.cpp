// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_HealthCheckService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TMS_LS/Components/TMS_HealthComponent.h"

UTMS_HealthCheckService::UTMS_HealthCheckService()
{
	NodeName = "Health Check";
}

void UTMS_HealthCheckService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	
	if (Controller && Controller->GetPawn())
	{
		if (UTMS_HealthComponent* HC = Controller->GetPawn()->GetComponentByClass<UTMS_HealthComponent>())
		{
			Blackboard->SetValueAsBool(DangerStateKey.SelectedKeyName,
				HC->GetHealthPercentage() < (SafeHealthPercentage / 100.f));
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
