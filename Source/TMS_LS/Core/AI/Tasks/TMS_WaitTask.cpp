#include "TMS_WaitTask.h"

#include "AIController.h"
#include "TMS_LS/Core/AI/TMS_EnemyCharacterBase.h"
#include "TMS_LS/Utilities/TMS_AIPatrolPath.h"

UTMS_WaitTask::UTMS_WaitTask()
{
	NodeName = "TMS Wait";
}

EBTNodeResult::Type UTMS_WaitTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	if (!Controller->GetPawn()) return EBTNodeResult::Failed;

	ATMS_EnemyCharacterBase* Char = Cast<ATMS_EnemyCharacterBase>(Controller->GetPawn());
	if (!Char || !Char->PatrolPath.Get()) return EBTNodeResult::Failed;

	WaitTime= FValueOrBBKey_Float(Char->PatrolPath.Get()->WaitArray[Char->PatrolPath.Get()->CurrentIndex]);

	return Super::ExecuteTask(OwnerComp, NodeMemory);
	
}