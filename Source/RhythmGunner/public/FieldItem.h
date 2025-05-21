// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "FieldItem.generated.h"

class USphereComponent;
class UNiagaraComponent;


UCLASS()
class RHYTHMGUNNER_API AFieldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFieldItem();

    UPROPERTY(VisibleAnywhere)
        USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
        UNiagaraComponent* NiagaraComponent;

    UPROPERTY(EditAnywhere, Category = "Effects")
        UNiagaraSystem* PickupEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult& SweepResult);

    virtual void ApplyItemFunction(ACharacter* _actor) { }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
