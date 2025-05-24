// ShootBoss.cpp
#include "ShootBoss.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

AShootBoss::AShootBoss()
{
	PrimaryActorTick.bCanEverTick = true;

	// �� �޽� ���� �� ��Ʈ�� �������� �ʰ� Skeletal Mesh�� ���Ͽ� ������ ����
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(GetMesh()); // �ϴ� �޽ÿ� ���̱�
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShootBoss::BeginPlay()
{
	Super::BeginPlay();

	// GunSocket�� �� ����
	if (GetMesh()->DoesSocketExist(TEXT("GunSocket")))
	{
		GunMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GunSocket"));
	}
}

void AShootBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. �÷��̾� �Ÿ� Ȯ��
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player) return;

	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	if (Distance > FireRange) return;  // �����Ÿ� ���̸� �ƹ� �͵� �� ��

	// 2. ��Ÿ�� üũ �� �Ѿ� �߻�
	FireTimer += DeltaTime;
	if (FireTimer >= FireInterval && BulletClass)
	{
		FireTimer = 0.f;

		// �ѱ� ��ġ ��������
		FVector MuzzleLocation = GunMesh->GetComponentLocation();
		FRotator MuzzleRotation = GunMesh->GetComponentRotation();

		// �Ѿ� �߻�
		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.Instigator = GetInstigator();
		GetWorld()->SpawnActor<AActor>(BulletClass, MuzzleLocation, MuzzleRotation, Params);

		// 3. �߻� �ִϸ��̼� ����
		if (ShootMontage)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && !AnimInstance->Montage_IsPlaying(ShootMontage))
			{
				AnimInstance->Montage_Play(ShootMontage);
			}
		}
	}
}
