// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (PlayerHUDClass)
    {
        PlayerHUDInstance = CreateWidget<UUserWidget>(this, PlayerHUDClass);
        if (PlayerHUDInstance)
        {
            PlayerHUDInstance->AddToViewport();

            // (선택) 마우스 커서 보이기 & 입력 모드 설정
            bShowMouseCursor = false;
            FInputModeGameOnly InputMode;
            SetInputMode(InputMode);
        }
    }
}
