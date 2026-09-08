// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "TMS_CharacterEQSC.generated.h"

/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_CharacterEQSC : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	FName TargetActorKeyName = "TargetActor";
};
