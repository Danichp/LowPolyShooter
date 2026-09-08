#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TMS_GetPatrolPathLocation.generated.h"

UCLASS()
class TMS_LS_API UTMS_GetPatrolPathLocation : public UBTTaskNode
{   
    GENERATED_BODY()
public:
    UTMS_GetPatrolPathLocation();
    
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    
protected:
    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector TargetLocationKey;
};