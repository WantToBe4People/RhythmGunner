// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FieldItemBase.h"
#include "FieldItem_HPAmplifier.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMGUNNER_API AFieldItem_HPAmplifier : public AFieldItemBase
{
	GENERATED_BODY()
	
public:
	AFieldItem_HPAmplifier();

protected:
	virtual void ApplyItemFunction(ACharacter* PlayerCharacter) override;

	UPROPERTY(EditAnywhere, Category = "Item|Effect")
	float MaxHPIncrement = 30.0f;  // 최대 체력 증가
};
