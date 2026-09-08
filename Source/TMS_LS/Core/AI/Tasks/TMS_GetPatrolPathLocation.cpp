#include "TMS_GetPatrolPathLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TMS_LS/Core/AI/TMS_EnemyCharacterBase.h"
#include "TMS_LS/Utilities/TMS_AIPatrolPath.h"

UTMS_GetPatrolPathLocation::UTMS_GetPatrolPathLocation()
{
	NodeName = "TMS Get Patrol Path Location";
}

EBTNodeResult::Type UTMS_GetPatrolPathLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !Blackboard) return EBTNodeResult::Failed;

	if (!Controller->GetPawn()) return EBTNodeResult::Failed;

	ATMS_EnemyCharacterBase* Char = Cast<ATMS_EnemyCharacterBase>(Controller->GetPawn());

	if (!Char || !Char->PatrolPath.Get()) return EBTNodeResult::Failed;

	const FVector Location = Char->PatrolPath.Get()->GetLocationAtCurrentIndex();

	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, Location);

	Char->PatrolPath.Get()->OnReachPoint();
	
	return EBTNodeResult::Succeeded;

}
    