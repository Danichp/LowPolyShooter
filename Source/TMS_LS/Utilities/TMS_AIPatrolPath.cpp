
#include "TMS_AIPatrolPath.h"

ATMS_AIPatrolPath::ATMS_AIPatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

void ATMS_AIPatrolPath::BeginPlay()
{
	Super::BeginPlay();
}

#if WITH_EDITOR
void ATMS_AIPatrolPath::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void ATMS_AIPatrolPath::InitWaitArray()
{
	WaitArray.SetNum(SplineComponent->GetNumberOfSplinePoints());
}