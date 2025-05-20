// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMGUNNER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
    virtual void BeginPlay() override;

protected:
    // HUD Ŭ���� ������
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> PlayerHUDClass;

private:
    // ���� ������ HUD �ν��Ͻ�
    UUserWidget* PlayerHUDInstance;
};
