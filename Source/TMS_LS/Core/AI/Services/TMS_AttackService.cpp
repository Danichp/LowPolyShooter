// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_AttackService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TMS_LS/Components/TMS_WeaponComponent.h"

UTMS_AttackService::UTMS_AttackService()
{
	NodeName = "Attack Service";
}

void UTMS_AttackService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	const bool bHasTarget = Blackboard && Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName);
	
	if (Controller && Controller->GetPawn())
	{
		const auto WC = Controller->GetPawn()->GetComponentByClass<UTMS_WeaponComponent>();
		if (WC)
		{
			WC->UseWeapon(EWeaponActionType::EWAT_Main, bHasTarget);
		}
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
