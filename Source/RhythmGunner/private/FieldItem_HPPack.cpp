// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldItem_HPPack.h"
#include "GameFramework/Character.h"
#include "../RhythmGunnerCharacter.h"

AFieldItem_HPPack::AFieldItem_HPPack()
{
}

void AFieldItem_HPPack::ApplyItemFunction(ACharacter* PlayerCharacter)
{
    /// ���� �÷��̾� Ŭ������ ĳ������ ��
    ARhythmGunnerCharacter* MyChar = Cast<ARhythmGunnerCharacter>(PlayerCharacter);
    if (MyChar)
    {
        MyChar->AddHealth(HealAmount); 
    }
}
