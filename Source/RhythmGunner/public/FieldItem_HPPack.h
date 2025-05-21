// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FieldItemBase.h"
#include "FieldItem_HPPack.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMGUNNER_API AFieldItem_HPPack : public AFieldItemBase
{
	GENERATED_BODY()
	
public:
	AFieldItem_HPPack();

protected:
	virtual void ApplyItemFunction(ACharacter* PlayerCharacter) override;

	UPROPERTY(EditAnywhere, Category = "Item|Effect")
	float HealAmount = 30.0f;  // 기본 회복량
};
