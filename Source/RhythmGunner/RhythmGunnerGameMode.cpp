// Copyright Epic Games, Inc. All Rights Reserved.

#include "RhythmGunnerGameMode.h"
#include "RhythmGunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

ARhythmGunnerGameMode::ARhythmGunnerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Tick 활성화
	PrimaryActorTick.bCanEverTick = true;

	bIsPaused = false;
	GameStartTime = 0.0f;
	GamePlayTime = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("GameMode Constructor Called"));
}

void ARhythmGunnerGameMode::BeginPlay()
{
	Super::BeginPlay();
	GameStartTime = GetWorld()->GetTimeSeconds();
	
	UE_LOG(LogTemp, Log, TEXT("Game Started at: %f"), GameStartTime);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Game Started!"));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, 
			TEXT("Ctrl + 1: 일시정지/재개\nCtrl + 2: 게임 재시작\nCtrl + 3: 게임 종료\nCtrl + 4: 현재 상태 표시"));
	}
}

void ARhythmGunnerGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GamePlayTime = GetWorld()->GetTimeSeconds() - GameStartTime;
	
	UE_LOG(LogTemp, Log, TEXT("Game Ended. Total Play Time: %f seconds"), GamePlayTime);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, 
			FString::Printf(TEXT("Game Ended. Play Time: %.2f seconds"), GamePlayTime));
	}
}

void ARhythmGunnerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return;

	// Ctrl 키가 눌려있는지 확인
	bool bCtrlPressed = PlayerController->IsInputKeyDown(EKeys::LeftControl) || 
					   PlayerController->IsInputKeyDown(EKeys::RightControl);

	if (bCtrlPressed)
	{
		// Ctrl + 1: 일시정지/재개
		if (PlayerController->WasInputKeyJustPressed(EKeys::One))
		{
			UE_LOG(LogTemp, Log, TEXT("Ctrl + 1 pressed - Toggling Pause"));
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Ctrl + 1 pressed"));
			}
			TogglePause();
		}
		// Ctrl + 2: 게임 재시작
		else if (PlayerController->WasInputKeyJustPressed(EKeys::Two))
		{
			UE_LOG(LogTemp, Log, TEXT("Ctrl + 2 pressed - Restarting Game"));
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Ctrl + 2 pressed"));
			}
			RestartGame();
		}
		// Ctrl + 3: 게임 종료
		else if (PlayerController->WasInputKeyJustPressed(EKeys::Three))
		{
			UE_LOG(LogTemp, Log, TEXT("Ctrl + 3 pressed - Quitting Game"));
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Ctrl + 3 pressed"));
			}
			QuitGame();
		}
		// Ctrl + 4: 현재 상태 표시
		else if (PlayerController->WasInputKeyJustPressed(EKeys::Four))
		{
			UE_LOG(LogTemp, Log, TEXT("Ctrl + 4 pressed - Showing Game Status"));
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Ctrl + 4 pressed"));
			}
			FString StatusMessage = FString::Printf(TEXT(
				"Game Status:\n"
				"Pause State: %s\n"
				"Play Time: %.2f seconds\n"
				"Game Start Time: %.2f seconds"),
				bIsPaused ? TEXT("Paused") : TEXT("Running"),
				GetWorld()->GetTimeSeconds() - GameStartTime,
				GameStartTime);

			UE_LOG(LogTemp, Log, TEXT("%s"), *StatusMessage);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, StatusMessage);
			}
		}
	}
}

void ARhythmGunnerGameMode::TogglePause()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		if (bIsPaused)
		{
			// 게임 재개
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Game Resumed"));
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Game Resumed"));
			}
		}
		else
		{
			// 게임 일시정지
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
			UE_LOG(LogTemp, Log, TEXT("Game Paused"));
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Game Paused"));
			}
		}
		bIsPaused = !bIsPaused;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Toggle Pause - PlayerController not found"));
	}
}

void ARhythmGunnerGameMode::RestartGame()
{
	UE_LOG(LogTemp, Log, TEXT("Restarting Game..."));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Restarting Game..."));
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
}

void ARhythmGunnerGameMode::QuitGame()
{
	UE_LOG(LogTemp, Log, TEXT("Quitting Game..."));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Quitting Game..."));
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->ConsoleCommand("quit");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Quit Game - PlayerController not found"));
	}
}
