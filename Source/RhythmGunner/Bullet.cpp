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
	// Query and Physics so OnHit �̺�Ʈ�� �߻�
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	// ���� ����ƽ(����/��)�� ���, Pawn(�÷��̾�/��)�� ���, �� ��(���� ���̳���)�� ���
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	// �Ѿ˳��� ��ġ�� �ϰ� ������ Pawn�� Block; �ʿ�� ����
	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	// 2) Mesh (�ð�)
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

	// 4) �ڵ� �ı�
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
	// �ڽ��� �� Actor(Owner)�ʹ� ����
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		// ������ �ֱ� (�÷��̾ ���� ���)
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

		// (Optional) ��ƼŬ ����Ʈ ���
		// if (HitEffect) UGameplayStatics::SpawnEmitterAtLocation(...);

		// �Ѿ� �ı�
		Destroy();
	}
}
