// Copyright Epic Games, Inc. All Rights Reserved.

#include "TriaGameMode.h"
#include "TriaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATriaGameMode::ATriaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
