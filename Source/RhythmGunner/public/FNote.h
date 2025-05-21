// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FNote.generated.h"

UCLASS()
class RHYTHMGUNNER_API AFNote : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFNote();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class UStaticMeshComponent* SphereMesh;
	UPROPERTY(EditAnywhere)
	int16 type = 0;
	UPROPERTY(EditAnywhere)
	double timing = 2.f;
};
