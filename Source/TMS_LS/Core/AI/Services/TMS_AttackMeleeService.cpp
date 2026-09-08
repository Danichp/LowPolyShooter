#include "TMS_AttackMeleeService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TMS_LS/Components/TMS_WeaponComponent.h"

class UTMS_WeaponComponent;

UTMS_AttackMeleeService::UTMS_AttackMeleeService()
{
	NodeName = "TMS Melee Attack Service";
}

void UTMS_AttackMeleeService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	
	if (Controller && Controller->GetPawn() && Target)
	{
		const float Dist = (Controller->GetPawn()->GetActorLocation() - Target->GetActorLocation()).Size();
		const auto WC = Controller->GetPawn()->GetComponentByClass<UTMS_WeaponComponent>();
		if (WC && Dist < AcceptableDistance)
		{
			WC->UseWeapon(EWeaponActionType::EWAT_Main, true);
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
