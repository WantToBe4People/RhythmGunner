// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldItemBase.generated.h"


class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class RHYTHMGUNNER_API AFieldItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFieldItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	virtual void ApplyItemFunction(ACharacter* PlayerCharacter) { };
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UNiagaraComponent* IdleEffectComponent;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	USoundBase* PickupSound;
};
