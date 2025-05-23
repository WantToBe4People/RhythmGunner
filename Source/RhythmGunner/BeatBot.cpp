#include "BeatBot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystem.h"

ABeatBot::ABeatBot()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1) ĸ�� �ݸ���: �÷��̾�� Overlap
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABeatBot::OnOverlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 2) �̵� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	// 3) �ð��� �޽� ������Ʈ
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetCapsuleComponent());
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABeatBot::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ABeatBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player) return;

	// ������ �߰�: �÷��̾���� �Ÿ� ��� ������
	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	if (Distance > DetectionRadius)
	{
		// �ݰ� ����� ���� ����
		return;
	}

	// Z���� �����ϰ� ���󿡼��� ���� ���
	FVector Dir = Player->GetActorLocation() - GetActorLocation();
	Dir.Z = 0;
	Dir.Normalize();
	FRotator LookRotation = Dir.Rotation();
	LookRotation.Yaw += 90.f; // ���� ����: -90��
	SetActorRotation(LookRotation);

	// CharacterMovementComponent�� ���� �浹�� ó���ϸ� ����
	AddMovementInput(Dir, 1.0f);
}

void ABeatBot::OnOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// **�÷��̾�� �浹���� ����** ����
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
