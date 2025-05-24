#include "ShootBullet.h"

/* .cpp에서만 실제 include */
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AShootBullet::AShootBullet()
{
    PrimaryActorTick.bCanEverTick = false;

    /* ── Collision ── */
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    SetRootComponent(CollisionComp);
    CollisionComp->InitSphereRadius(10.f);

    /* BlockAll preset + Hit 이벤트 */
    CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));
    CollisionComp->SetNotifyRigidBodyCollision(true);
    CollisionComp->OnComponentHit.AddDynamic(this, &AShootBullet::OnHit);

    /* ── Mesh ── */
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(CollisionComp);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    /* ── Projectile Movement ── */
    MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
    MoveComp->InitialSpeed = 1500.f;
    MoveComp->MaxSpeed = 1500.f;
    MoveComp->bRotationFollowsVelocity = true;
    MoveComp->ProjectileGravityScale = 0.f;

    InitialLifeSpan = 3.f;
}

void AShootBullet::OnHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector              NormalImpulse,
    const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        ExplodeAndDestroy();
    }
}

void AShootBullet::ExplodeAndDestroy()
{
    if (HitEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            HitEffect,
            GetActorLocation(),
            GetActorRotation(),
            true);          // Finished 후 자동 파괴
    }
    Destroy();
}
