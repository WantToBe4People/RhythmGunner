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
	RotationAxis = FVector::RightVector; // �ʱ� X�� ȸ��
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
	// ���ο� �������� ���� �� ����
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
	RotationAxis = FVector::RightVector; // �ʱ� X�� ȸ��
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
	/* --------- Firing ó�� --------- */
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


/* ===== Firing: ���� �߻� ����(3~8�� ����) ��� ===== */
void ARollingCube::ScheduleNextFire()
{
	NextFireInterval = FMath::FRandRange(MinFireInterval, MaxFireInterval);
}

/* ===== Firing: 8�������� �Ѿ� ���� (�浹 ���� + ������ + �ӵ� ����) ===== */
void ARollingCube::FireProjectilesInEightDirections()
{
	if (!ProjectileClass) return;

	const float SpawnDist = 60.f;      // ť�� ǥ�鿡�� 60���� ������ ����
	const float BulletSpeed = 1500.f;    // �Ѿ� �ʱ� �ӵ�

	FVector Center = GetActorLocation() + FireOffset;

	const TArray<FVector> Directions = {
		FVector(1,  0, 0),  FVector(1,  1, 0).GetSafeNormal(),
		FVector(0,  1, 0),  FVector(-1,  1, 0).GetSafeNormal(),
		FVector(-1,  0, 0),  FVector(-1, -1, 0).GetSafeNormal(),
		FVector(0, -1, 0),  FVector(1, -1, 0).GetSafeNormal()
	};

	for (const FVector& Dir : Directions)
	{
		/*  ���� ��ġ = ť�� �߽� + FireOffset + (���� * SpawnDist) */
		FVector SpawnLoc = Center + Dir * SpawnDist;
		FRotator SpawnRot = Dir.Rotation();

		/*  �浹 �����ϰ� �׻� ���� */
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* Bullet = GetWorld()->SpawnActor<AActor>(
			ProjectileClass, SpawnLoc, SpawnRot, Params);

		/*  ProjectileMovement ������ ���� �ӵ� ���� */
		if (Bullet)
		{
			if (auto Move = Bullet->FindComponentByClass<UProjectileMovementComponent>())
			{
				Move->Velocity = Dir * BulletSpeed;
			}
		}
	}
}
