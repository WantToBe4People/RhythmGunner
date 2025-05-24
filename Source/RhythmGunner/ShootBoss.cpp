// ShootBoss.cpp
#include "ShootBoss.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

AShootBoss::AShootBoss()
{
	PrimaryActorTick.bCanEverTick = true;

	// 총 메시 생성 및 루트에 부착하지 않고 Skeletal Mesh의 소켓에 부착할 예정
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(GetMesh()); // 일단 메시에 붙이기
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShootBoss::BeginPlay()
{
	Super::BeginPlay();

	// GunSocket에 총 부착
	if (GetMesh()->DoesSocketExist(TEXT("GunSocket")))
	{
		GunMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GunSocket"));
	}
}

void AShootBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. 플레이어 거리 확인
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Player) return;

	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	if (Distance > FireRange) return;  // 사정거리 밖이면 아무 것도 안 함

	// 2. 쿨타임 체크 및 총알 발사
	FireTimer += DeltaTime;
	if (FireTimer >= FireInterval && BulletClass)
	{
		FireTimer = 0.f;

		// 총구 위치 가져오기
		FVector MuzzleLocation = GunMesh->GetComponentLocation();
		FRotator MuzzleRotation = GunMesh->GetComponentRotation();

		// 총알 발사
		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.Instigator = GetInstigator();
		GetWorld()->SpawnActor<AActor>(BulletClass, MuzzleLocation, MuzzleRotation, Params);

		// 3. 발사 애니메이션 실행
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
