#include "BeatBot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystem.h"

ABeatBot::ABeatBot()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1) 캡슐 콜리전: 플레이어에만 Overlap
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABeatBot::OnOverlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 2) 이동 세팅
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	// 3) 시각용 메쉬 컴포넌트
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

	// ─── 추가: 플레이어와의 거리 계산 ───
	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	if (Distance > DetectionRadius)
	{
		// 반경 벗어나면 추적 중지
		return;
	}

	// Z축은 고정하고 평면상에서만 방향 계산
	FVector Dir = Player->GetActorLocation() - GetActorLocation();
	Dir.Z = 0;
	Dir.Normalize();
	FRotator LookRotation = Dir.Rotation();
	LookRotation.Yaw += 90.f; // 정면 보정: -90도
	SetActorRotation(LookRotation);

	// CharacterMovementComponent가 물리 충돌을 처리하며 추적
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
	// **플레이어와 충돌했을 때만** 반응
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
