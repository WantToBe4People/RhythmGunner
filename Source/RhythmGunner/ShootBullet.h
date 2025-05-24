#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootBullet.generated.h"

/* ────────── forward declarations (순환 include 방지) ────────── */
class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class RHYTHMGUNNER_API AShootBullet : public AActor
{
    GENERATED_BODY()

public:
    AShootBullet();

protected:
    /* Block 충돌 시 */
    UFUNCTION()
        void OnHit(
            UPrimitiveComponent* HitComp,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            FVector              NormalImpulse,
            const FHitResult& Hit);

private:
    /* ── 컴포넌트 ── */
    UPROPERTY(VisibleAnywhere) USphereComponent* CollisionComp;
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* MeshComp;
    UPROPERTY(VisibleAnywhere) UProjectileMovementComponent* MoveComp;

    /* 폭발 파티클 (BP에서 지정) */
    UPROPERTY(EditAnywhere, Category = "Bullet|FX") UParticleSystem* HitEffect;

    void ExplodeAndDestroy();
};
