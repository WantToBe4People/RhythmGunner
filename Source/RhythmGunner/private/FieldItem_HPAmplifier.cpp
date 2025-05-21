// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldItem_HPAmplifier.h"
#include "RhythmGunnerCharacter.h"

AFieldItem_HPAmplifier::AFieldItem_HPAmplifier()
{
}

void AFieldItem_HPAmplifier::ApplyItemFunction(ACharacter* PlayerCharacter)
{
    /// 추후 플레이어 클래스로 캐스팅할 것
    ARhythmGunnerCharacter* MyChar = Cast<ARhythmGunnerCharacter>(PlayerCharacter);
    if (MyChar)
    {
        MyChar->AddPlayerMAXHP(MaxHPIncrement);
        MyChar->AddHealth(1000); 
    }
}
