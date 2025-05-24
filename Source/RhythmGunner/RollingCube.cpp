/*
#include "RollingCube.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARollingCube::ARollingCube()
{
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	SetRootComponent(CubeMesh);
	CubeMesh->SetSimulatePhysics(false);
	CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CubeMesh->SetCollisionObjectType(ECC_PhysicsBody);
}

void ARollingCube::BeginPlay()
{
	Super::BeginPlay();
	StartRotation = GetActorRotation();
	StartLocation = GetActorLocation();
	RotationAxis = FVector::RightVector; // 초기 X축 회전
	bIsRolling = false;
	CurrentAngle = 0.0f;
	TimeSinceLastRoll = 0.0f;
}

void ARollingCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastRoll += DeltaTime;

	if (!bIsRolling && TimeSinceLastRoll >= RollInterval)
	{
		StartRoll();
		TimeSinceLastRoll = 0.0f;
	}

	if (bIsRolling)
	{
		float DeltaAngle = RollSpeed * DeltaTime;
		CurrentAngle += DeltaAngle;

		if (CurrentAngle >= RollAngle)
		{
			DeltaAngle -= (CurrentAngle - RollAngle);
			bIsRolling = false;
			CurrentAngle = 0.0f;
		}

		FQuat QuatRotation = FQuat(RotationAxis, FMath::DegreesToRadians(DeltaAngle));
		AddActorWorldRotation(QuatRotation);
	}
}

void ARollingCube::StartRoll()
{
	// 새로운 방향으로 굴림 축 변경
	int32 RandomAxis = FMath::RandRange(0, 3);
	switch (RandomAxis)
	{
	case 0:
		RotationAxis = FVector::RightVector;
		break;
	case 1:
		RotationAxis = FVector::LeftVector;
		break;
	case 2:
		RotationAxis = FVector::ForwardVector;
		break;
	case 3:
		RotationAxis = FVector::BackwardVector;
		break;
	}

	bIsRolling = true;
	CurrentAngle = 0.0f;
}
*/

#include "RollingCube.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ARollingCube::ARollingCube()
{
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	SetRootComponent(CubeMesh);
	CubeMesh->SetSimulatePhysics(false);
	CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CubeMesh->SetCollisionObjectType(ECC_PhysicsBody);
}

void ARollingCube::BeginPlay()
{
	Super::BeginPlay();

	StartRotation = GetActorRotation();
	RotationAxis = FVector::RightVector; // 초기 X축 회전
	CurrentAngle = 0.0f;
	TimeSinceLastRoll = 0.0f;
	bIsRolling = false;

	TimeSinceLastFire = 0.0f;         // ========== Firing ==========
	ScheduleNextFire();               // ========== Firing ==========

	
}

void ARollingCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastRoll += DeltaTime;

	if (!bIsRolling && TimeSinceLastRoll >= RollInterval)
	{
		StartRoll();
		TimeSinceLastRoll = 0.0f;
	}

	if (bIsRolling)
	{
		float DeltaAngle = RollSpeed * DeltaTime;
		CurrentAngle += DeltaAngle;

		if (CurrentAngle >= RollAngle)
		{
			DeltaAngle -= (CurrentAngle - RollAngle);
			bIsRolling = false;
			CurrentAngle = 0.0f;
		}

		FQuat QuatRotation = FQuat(RotationAxis, FMath::DegreesToRadians(DeltaAngle));
		AddActorWorldRotation(QuatRotation);
	}
	/* --------- Firing 처리 --------- */
	TimeSinceLastFire += DeltaTime;                       // ========== Firing ==========
	if (TimeSinceLastFire >= NextFireInterval)            // ========== Firing ==========
	{
		FireProjectilesInEightDirections();               // ========== Firing ==========
		ScheduleNextFire();                               // ========== Firing ==========
		TimeSinceLastFire = 0.0f;                         // ========== Firing ==========
	}
}

void ARollingCube::StartRoll()
{
	int32 Rand = FMath::RandRange(0, 3);
	switch (Rand)
	{
	case 0: RotationAxis = FVector::RightVector;   break;
	case 1: RotationAxis = FVector::LeftVector;    break;
	case 2: RotationAxis = FVector::ForwardVector; break;
	case 3: RotationAxis = FVector::BackwardVector; break;
	}
	bIsRolling = true;
	CurrentAngle = 0.0f;
}


/* ===== Firing: 다음 발사 간격(3~8초 랜덤) 계산 ===== */
void ARollingCube::ScheduleNextFire()
{
	NextFireInterval = FMath::FRandRange(MinFireInterval, MaxFireInterval);
}

/* ===== Firing: 8방향으로 총알 스폰 (충돌 무시 + 오프셋 + 속도 주입) ===== */
void ARollingCube::FireProjectilesInEightDirections()
{
	if (!ProjectileClass) return;

	const float SpawnDist = 60.f;      // 큐브 표면에서 60유닛 떨어져 스폰
	const float BulletSpeed = 1500.f;    // 총알 초기 속도

	FVector Center = GetActorLocation() + FireOffset;

	const TArray<FVector> Directions = {
		FVector(1,  0, 0),  FVector(1,  1, 0).GetSafeNormal(),
		FVector(0,  1, 0),  FVector(-1,  1, 0).GetSafeNormal(),
		FVector(-1,  0, 0),  FVector(-1, -1, 0).GetSafeNormal(),
		FVector(0, -1, 0),  FVector(1, -1, 0).GetSafeNormal()
	};

	for (const FVector& Dir : Directions)
	{
		/*  스폰 위치 = 큐브 중심 + FireOffset + (방향 * SpawnDist) */
		FVector SpawnLoc = Center + Dir * SpawnDist;
		FRotator SpawnRot = Dir.Rotation();

		/*  충돌 무시하고 항상 생성 */
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* Bullet = GetWorld()->SpawnActor<AActor>(
			ProjectileClass, SpawnLoc, SpawnRot, Params);

		/*  ProjectileMovement 있으면 방향 속도 주입 */
		if (Bullet)
		{
			if (auto Move = Bullet->FindComponentByClass<UProjectileMovementComponent>())
			{
				Move->Velocity = Dir * BulletSpeed;
			}
		}
	}
}
