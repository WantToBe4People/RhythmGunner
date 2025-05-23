
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

	// ĸ�� ������Ʈ ����
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInvisibleEnemy::OnOverlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// �ð��� �޽� ������Ʈ
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetCapsuleComponent());
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �̵� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void AInvisibleEnemy::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ToggleVisibility(false); // ���� �� ����
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

	// ȸ�� + �̵��� �׻� ���� (������ �־)
	FVector Dir = Player->GetActorLocation() - GetActorLocation();
	Dir.Z = 0;
	Dir.Normalize();
	FRotator LookRotation = Dir.Rotation();
	LookRotation.Yaw -= 90.f; // ���� ����
	SetActorRotation(LookRotation);
	AddMovementInput(Dir, 1.0f);

	// ���� ���� ��ȯ ������ ó��
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

	// �޽ø� ����� �̵��� �浹�� ����
	MeshComp->SetVisibility(!bHide, true); // �ڽ� �޽ñ��� ���� ó��
}



