// RollingCube.h
#pragma once
/*

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RollingCube.generated.h"

UCLASS()
class RHYTHMGUNNER_API ARollingCube : public AActor
{
	GENERATED_BODY()

public:
	ARollingCube();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float RollAngle = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float RollSpeed = 90.0f; // degrees per second

	UPROPERTY(EditAnywhere, Category = "Movement")
		float RollInterval = 1.0f; // seconds

	bool bIsRolling = false;
	float CurrentAngle = 0.0f;
	float TimeSinceLastRoll = 0.0f;
	FRotator StartRotation;
	FVector StartLocation;
	FVector RotationAxis;

	void StartRoll();
};*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RollingCube.generated.h"

UCLASS()
class RHYTHMGUNNER_API ARollingCube : public AActor
{
	GENERATED_BODY()

public:
	ARollingCube();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditAnywhere, Category = "Rolling")
		float RollInterval = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Rolling")
		float RollSpeed = 90.0f; // degrees per second

	UPROPERTY(EditAnywhere, Category = "Rolling")
		float RollAngle = 90.0f;

	FRotator StartRotation;
	FVector RotationAxis;
	float CurrentAngle;
	float TimeSinceLastRoll;
	bool bIsRolling;

	void StartRoll();

	UPROPERTY(EditAnywhere, Category = "Firing")
		TSubclassOf<AActor> ProjectileClass;

	// ========== Firing: ���� ���� ����(��) ==========
	UPROPERTY(EditAnywhere, Category = "Firing")
		float MinFireInterval = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Firing")
		float MaxFireInterval = 8.0f;

	// ========== Firing: �߻� ��ġ ������ ==========
	UPROPERTY(EditAnywhere, Category = "Firing")
		FVector FireOffset = FVector(0, 0, 50);

	// ========== Firing: ���� Ÿ�̸� / ���� �߻���� ���� �ð� ==========
	float TimeSinceLastFire = 0.0f;
	float NextFireInterval = 0.0f;

	// ========== Firing: �Լ� ==========
	void ScheduleNextFire();               // ���� ���� ���� ���
	void FireProjectilesInEightDirections(); // 8���� �߻�
};


