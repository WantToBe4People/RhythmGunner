// Copyright Epic Games, Inc. All Rights Reserved.

#include "RhythmGunnerGameMode.h"
#include "RhythmGunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "RhythmSubsystem.h"

ARhythmGunnerGameMode::ARhythmGunnerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BluePrints/BP_RhythmGunnerCharacter.BP_RhythmGunnerCharacter_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ARhythmGunnerGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetupInputBindings();
	DisplayShortcutInfo();
}

void ARhythmGunnerGameMode::SetupInputBindings()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// Ctrl + 1: ?¼ì‹œ?•ì?/?¬ê°œ
		PlayerController->InputComponent->BindKey(FKey("One"), EInputEvent::IE_Pressed, this, &ARhythmGunnerGameMode::TogglePause).bExecuteWhenPaused = true;
		// Ctrl + 2: ?ˆë²¨ ?¬ì‹œ??
		//PlayerController->InputComponent->BindKey(FKey("Two"), EInputEvent::IE_Pressed, this, &ARhythmGunnerGameMode::RestartLevel).bExecuteWhenPaused = true;
	}
}

void ARhythmGunnerGameMode::TogglePause()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (PlayerController->IsPaused())
		{
			ResumeGame();
		}
		else
		{
			PauseGame();
		}
	}
}

void ARhythmGunnerGameMode::PauseGame()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController->SetPause(true);
	}
}

void ARhythmGunnerGameMode::ResumeGame()
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController->SetPause(false);
	}
}

void ARhythmGunnerGameMode::RestartLevel()
{
	
}

void ARhythmGunnerGameMode::DisplayShortcutInfo()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("?¨ì¶•???ˆë‚´:"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("Ctrl + 1: ê²Œìž„ ?¼ì‹œ?•ì?/?¬ê°œ"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("Ctrl + 2: ?ˆë²¨ ?¬ì‹œ??(?„ìž¬ ë¹„í™œ?±í™”)"));
	}
}
