// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_CharacterEQSC.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UTMS_CharacterEQSC::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());
	const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);

	if (!QueryOwner || !Blackboard) return;

	const auto ContextActor = Blackboard->GetValueAsObject(TargetActorKeyName);
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(ContextActor));
	
	Super::ProvideContext(QueryInstance, ContextData);
}
