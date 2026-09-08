// Copyright Epic Games, Inc. All Rights Reserved.

#include "TMS_LSGameMode.h"
#include "TMS_LSCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATMS_LSGameMode::ATMS_LSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
