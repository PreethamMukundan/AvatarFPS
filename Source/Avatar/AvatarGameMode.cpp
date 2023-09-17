// Copyright Epic Games, Inc. All Rights Reserved.

#include "AvatarGameMode.h"
#include "AvatarCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAvatarGameMode::AAvatarGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
