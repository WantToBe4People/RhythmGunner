// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RhythmGunnerGameMode.generated.h"

UCLASS(minimalapi)
class ARhythmGunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARhythmGunnerGameMode();

	// 게임 일시정지
	UFUNCTION(BlueprintCallable, Category = "Game")
	void PauseGame();

	// 게임 재개
	UFUNCTION(BlueprintCallable, Category = "Game")
	void ResumeGame();

	// 현재 레벨 재시작
	UFUNCTION(BlueprintCallable, Category = "Game")
	void RestartLevel();

protected:
	virtual void BeginPlay() override;

	// 입력 바인딩 설정
	void SetupInputBindings();

	// 일시정지 토글
	void TogglePause();

	// 단축키 정보 표시
	void DisplayShortcutInfo();
};



