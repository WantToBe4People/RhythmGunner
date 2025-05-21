// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDUserWidget.h"
#include "../RhythmGunnerCharacter.h"
#include <Kismet\GameplayStatics.h>
#include "Components/ProgressBar.h"

float UPlayerHUDUserWidget::GetPlayerHealthPercent() const
{
    ARhythmGunnerCharacter* MyChar = Cast<ARhythmGunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (MyChar)
    {
        return MyChar->GetPlayerCurrentHP() / MyChar->GetPlayerMaxHP();
    }
	return 0.10f;
}

void UPlayerHUDUserWidget::SetBossHPBarVisible(bool _value)
{
    if (BossHPBar)
    {
        BossHPBar->SetVisibility(_value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

float UPlayerHUDUserWidget::GetBossHealthPercent() const
{
    ARhythmGunnerCharacter* BossChar = Cast<ARhythmGunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (BossChar)
    {
        //return BossChar->CurrentHealth / BossChar->MaxHealth;
    }
    return 1.0f;
}
