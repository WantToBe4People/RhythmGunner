#include "ShooterEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Bullet.h"

AShooterEnemy::AShooterEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1) ĸ�� �ݸ��� ����
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AShooterEnemy::OnOverlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 2) �̵� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	// 3) �ð��� �޽� ������Ʈ
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetCapsuleComponent());
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShooterEnemy::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void AShooterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player) return;

	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	if (Distance > DetectionRadius) return;

	FVector Dir = Player->GetActorLocation() - GetActorLocation();
	Dir.Z = 0;
	Dir.Normalize();
	FRotator LookRotation = Dir.Rotation();
	LookRotation.Yaw -= 90.f; // ���� ����: -90��
	SetActorRotation(LookRotation);

	AddMovementInput(Dir, 1.0f);

	// ������ �߰�: ���� �������θ� �߻� ������
	CurrentFireTime += DeltaTime;
	if (CurrentFireTime >= FireInterval && BulletClass)
	{
		FVector MuzzleLocation = GetActorLocation() + FVector(100, 0, 50); // �Ѿ� ���� ��ġ
		FRotator MuzzleRotation = (Player->GetActorLocation() - MuzzleLocation).Rotation();

		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.Instigator = GetInstigator();

		GetWorld()->SpawnActor<AActor>(BulletClass, MuzzleLocation, MuzzleRotation, Params);

		CurrentFireTime = 0.f; // ��Ÿ�� �ʱ�ȭ
	}
}

void AShooterEnemy::OnOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor == Player)
	{
		if (HitEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				HitEffect,
				GetActorLocation()
			);
		}
		Destroy();
	}
}
