// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_PlayerController.h"

#include "Data/TMS_DataTypes.h"

FGenericTeamId ATMS_PlayerController::GetGenericTeamId() const
{
	return static_cast<uint8>(ETeamType::ETT_Player);
}

ETeamAttitude::Type ATMS_PlayerController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
	{
		const FGenericTeamId OtherGenericTeamId = TeamAgent->GetGenericTeamId();

		if (OtherGenericTeamId == FGenericTeamId(StaticCast<uint8>(ETeamType::ETT_NPC)))
			return ETeamAttitude::Friendly;

		if (OtherGenericTeamId == FGenericTeamId(StaticCast<uint8>(ETeamType::ETT_Bandits)) ||
			OtherGenericTeamId == FGenericTeamId(StaticCast<uint8>(ETeamType::ETT_Mercenaries)))
			return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Neutral;
}
