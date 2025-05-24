#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootBullet.generated.h"

/* �������������������� forward declarations (��ȯ include ����) �������������������� */
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
    /* Block �浹 �� */
    UFUNCTION()
        void OnHit(
            UPrimitiveComponent* HitComp,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            FVector              NormalImpulse,
            const FHitResult& Hit);

private:
    /* ���� ������Ʈ ���� */
    UPROPERTY(VisibleAnywhere) USphereComponent* CollisionComp;
    UPROPERTY(VisibleAnywhere) UStaticMeshComponent* MeshComp;
    UPROPERTY(VisibleAnywhere) UProjectileMovementComponent* MoveComp;

    /* ���� ��ƼŬ (BP���� ����) */
    UPROPERTY(EditAnywhere, Category = "Bullet|FX") UParticleSystem* HitEffect;

    void ExplodeAndDestroy();
};
