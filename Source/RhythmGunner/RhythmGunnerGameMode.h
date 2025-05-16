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

	// 게임 일시정지 토글
	UFUNCTION(BlueprintCallable, Category = "Game")
	void TogglePause();

	// 게임 재시작
	UFUNCTION(BlueprintCallable, Category = "Game")
	void RestartGame();

	// 게임 종료
	UFUNCTION(BlueprintCallable, Category = "Game")
	void QuitGame();

	// 게임 상태 확인
	UFUNCTION(BlueprintPure, Category = "Game")
	bool IsGamePaused() const { return bIsPaused; }

protected:
	// 게임 시작 시 호출
	virtual void BeginPlay() override;

	// 게임 종료 시 호출
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 틱 처리
	virtual void Tick(float DeltaTime) override;

private:
	// 게임 일시정지 상태
	UPROPERTY()
	bool bIsPaused;

	// 게임 시작 시간
	UPROPERTY()
	float GameStartTime;

	// 게임 플레이 시간
	UPROPERTY()
	float GamePlayTime;
};



