// Copyright Epic Games, Inc. All Rights Reserved.

#include "FantasyWorldGameMode.h"
#include "FantasyWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFantasyWorldGameMode::AFantasyWorldGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
