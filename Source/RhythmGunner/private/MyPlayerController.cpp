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

            // (����) ���콺 Ŀ�� ���̱� & �Է� ��� ����
            bShowMouseCursor = false;
            FInputModeGameOnly InputMode;
            SetInputMode(InputMode);
        }
    }
}
