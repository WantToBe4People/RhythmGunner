// Copyright Epic Games, Inc. All Rights Reserved.

#include "RhythmGunnerGameMode.h"
#include "RhythmGunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARhythmGunnerGameMode::ARhythmGunnerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
