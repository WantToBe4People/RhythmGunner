#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1) Collision
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);
	CollisionComp->InitSphereRadius(10.f);
	// Query and Physics so OnHit 이벤트가 발생
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	// 월드 스태틱(지형/벽)에 블록, Pawn(플레이어/적)에 블록, 그 외(월드 다이내믹)에 블록
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	// 총알끼리 겹치게 하고 싶으면 Pawn만 Block; 필요시 조정
	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	// 2) Mesh (시각)
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 3) Projectile Movement
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	MoveComp->UpdatedComponent = CollisionComp;
	MoveComp->InitialSpeed = 1200.f;
	MoveComp->MaxSpeed = 1200.f;
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bShouldBounce = false;

	// 4) 자동 파괴
	InitialLifeSpan = LifeSeconds;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	// 자신을 쏜 Actor(Owner)와는 무시
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		// 데미지 주기 (플레이어나 적일 경우)
		if (OtherActor->IsA<ACharacter>())
		{
			UGameplayStatics::ApplyDamage(
				OtherActor,
				Damage,
				GetInstigatorController(),
				this,
				nullptr
			);
		}

		// (Optional) 파티클 이펙트 재생
		// if (HitEffect) UGameplayStatics::SpawnEmitterAtLocation(...);

		// 총알 파괴
		Destroy();
	}
}
