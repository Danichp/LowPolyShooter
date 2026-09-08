#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "TMS_AIPatrolPath.generated.h"

UCLASS()
class TMS_LS_API ATMS_AIPatrolPath : public AActor
{
	GENERATED_BODY()
public:
	ATMS_AIPatrolPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> WaitArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentIndex = 0;
	
	int32 Dir = 1;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetLocationAtCurrentIndex()
	{
		CurrentIndex = FMath::Clamp(CurrentIndex, 0, SplineComponent->GetNumberOfSplinePoints() - 1);
		return SplineComponent->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
	}

	UFUNCTION(BlueprintCallable, CallInEditor)
	void InitWaitArray();

	UFUNCTION(BlueprintCallable)
	void OnReachPoint()
	{
		if (SplineComponent->IsClosedLoop())
		{
			CurrentIndex = (CurrentIndex+1) % SplineComponent->GetNumberOfSplinePoints();
		}
		else
		{
			int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
			if(NumPoints <= 1) return;
			
			CurrentIndex += Dir;
			
			if (CurrentIndex >= NumPoints)
			{
				CurrentIndex = NumPoints - 2;
				Dir = -1;
			}
			else if (CurrentIndex < 0)
			{
				CurrentIndex = 1;
				Dir = 1;
			}
		}
	}
};
