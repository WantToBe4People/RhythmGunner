// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMGUNNER_API UPlayerHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetPlayerHealthPercent() const;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BossHPBar;

	UFUNCTION(BlueprintCallable)
	void SetBossHPBarVisible(bool _value);

	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetBossHealthPercent() const;

private:
};
