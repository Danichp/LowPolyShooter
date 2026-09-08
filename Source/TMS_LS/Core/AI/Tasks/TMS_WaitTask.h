#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "TMS_WaitTask.generated.h"

UCLASS()
class TMS_LS_API UTMS_WaitTask : public UBTTask_Wait
{
	GENERATED_BODY()

	UTMS_WaitTask();
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
