#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TMS_AttackMeleeService.generated.h"


UCLASS()
class TMS_LS_API UTMS_AttackMeleeService : public UBTService
{
	GENERATED_BODY()
public:
	UTMS_AttackMeleeService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AcceptableDistance = 100.f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};