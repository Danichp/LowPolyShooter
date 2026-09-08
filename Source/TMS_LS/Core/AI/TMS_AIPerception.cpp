// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_AIPerception.h"

#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "TMS_LS/Components/TMS_HealthComponent.h"


TWeakObjectPtr<AActor> UTMS_AIPerception::GetClosestEnemy()
{
	TArray<AActor*> PerceivedActors;

	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	if (PerceivedActors.Num() == 0) return nullptr;

	const auto Cont = Cast<AAIController>(GetOwner());
	if (!Cont) return nullptr;

	const auto Pawn = Cont->GetPawn();
	if (!Pawn) return nullptr;

	float MinDistance = MAX_FLT;
	AActor* MinDistanceActor = nullptr;

	for (const auto Actor : PerceivedActors)
	{
		const UTMS_HealthComponent* HealthComp = Actor->GetComponentByClass<UTMS_HealthComponent>();
		if (HealthComp && !HealthComp->bDead)
		{
			const float Dist = (Actor->GetActorLocation() - Pawn->GetActorLocation()).SizeSquared();
			if (Dist < MinDistance)
			{
				MinDistance = Dist;
				MinDistanceActor = Actor;
			}
		}
	}
	return MinDistanceActor;
}
