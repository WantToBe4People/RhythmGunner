
#include "InvisibleEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

AInvisibleEnemy::AInvisibleEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(60.f, 80.f);

	// 캡슐 컴포넌트 설정
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInvisibleEnemy::OnOverlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 시각용 메시 컴포넌트
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetCapsuleComponent());
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 이동 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void AInvisibleEnemy::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ToggleVisibility(false); // 시작 시 보임
}

void AInvisibleEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AInvisibleEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player) return;

	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	if (Distance > DetectionRadius) return;

	// 회전 + 이동은 항상 진행 (숨겨져 있어도)
	FVector Dir = Player->GetActorLocation() - GetActorLocation();
	Dir.Z = 0;
	Dir.Normalize();
	FRotator LookRotation = Dir.Rotation();
	LookRotation.Yaw -= 90.f; // 정면 보정
	SetActorRotation(LookRotation);
	AddMovementInput(Dir, 1.0f);

	// 숨김 상태 전환 로직만 처리
	HiddenTimer += DeltaTime;
	if (HiddenTimer >= HiddenDuration)
	{
		ToggleVisibility(!bIsHidden);
		HiddenTimer = 0.0f;
	}
}

void AInvisibleEnemy::OnOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == Player)
	{
		if (HitEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorLocation());
		}
		Destroy();
	}
}

void AInvisibleEnemy::ToggleVisibility(bool bHide)
{
	bIsHidden = bHide;

	// 메시만 숨기고 이동과 충돌은 유지
	MeshComp->SetVisibility(!bHide, true); // 자식 메시까지 숨김 처리
}



