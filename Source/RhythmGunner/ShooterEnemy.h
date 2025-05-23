#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Components/StaticMeshComponent.h"
#include "ShooterEnemy.generated.h"

class ABullet;

UCLASS()
class RHYTHMGUNNER_API AShooterEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float MoveSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0"))
		float DetectionRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class AActor> BulletClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float FireInterval = 1.5f;  // 발사 간격

private:
	UPROPERTY()
		ACharacter* Player;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MeshComp;

	float CurrentFireTime = 0.f;  // 누적 시간 (총알 발사 쿨타임용)
};
