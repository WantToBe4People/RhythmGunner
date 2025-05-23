#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Components/StaticMeshComponent.h"
#include "BeatBot.generated.h"

UCLASS()
class RHYTHMGUNNER_API ABeatBot : public ACharacter
{
	GENERATED_BODY()

public:
	ABeatBot();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 플레이어와 부딪혔을 때 호출
	UFUNCTION()
		void OnOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	// 블루프린트에서 할당할 폭발 이펙트
	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* HitEffect;

	// 이동 속도
	UPROPERTY(EditAnywhere, Category = "Movement")
		float MoveSpeed = 400.0f;

	// ─── 추가: 이 반경 이내일 때만 추적 ───
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0"))
		float DetectionRadius = 1000.0f;

private:
	// 추적 대상 플레이어
	UPROPERTY()
		ACharacter* Player;

	// ─── 추가: 시각용 Static Mesh ───
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MeshComp;
};
