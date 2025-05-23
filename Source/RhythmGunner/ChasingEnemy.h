#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChasingEnemy.generated.h"

UCLASS()
class RHYTHMGUNNER_API AChasingEnemy : public AActor
{
	GENERATED_BODY()

public:
	AChasingEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float MoveSpeed = 250.0f;

private:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MeshComp;

	UPROPERTY()
		ACharacter* Player;
};
