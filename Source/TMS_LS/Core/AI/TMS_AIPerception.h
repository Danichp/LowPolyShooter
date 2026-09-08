// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "TMS_AIPerception.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TMS_LS_API UTMS_AIPerception : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<AActor> GetClosestEnemy();
};
