
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Components/StaticMeshComponent.h"
#include "InvisibleEnemy.generated.h"

UCLASS()
class RHYTHMGUNNER_API AInvisibleEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AInvisibleEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

private:
	void ToggleVisibility(bool bHide);

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MeshComp;

	UPROPERTY()
		ACharacter* Player;

	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float MoveSpeed = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float DetectionRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Invisibility")
		float HiddenDuration = 1.0f;

	float HiddenTimer = 0.0f;
	bool bIsHidden = false;
};
