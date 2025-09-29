// Copyright Epic Games, Inc. All Rights Reserved.

#include "ctlqkfGameMode.h"
#include "ctlqkfCharacter.h"
#include "UObject/ConstructorHelpers.h"

ActlqkfGameMode::ActlqkfGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
