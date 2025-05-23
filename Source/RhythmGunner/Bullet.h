#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class RHYTHMGUNNER_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// ������
	ABullet();

protected:
	// ���� ����
	virtual void BeginPlay() override;

	// �浹 �� ȣ��
	UFUNCTION()
		void OnHit(
			UPrimitiveComponent* HitComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit
		);

public:
	// �� �������� �ʿ� ������ ���μŵ� �˴ϴ�
	virtual void Tick(float DeltaTime) override;

	// ������ �� (�������Ʈ ���� ����)
	UPROPERTY(EditAnywhere, Category = "Bullet")
		float Damage = 10.0f;

	// (Optional) ���� �ð� �ʰ� �� �ڵ� �ı�
	UPROPERTY(EditAnywhere, Category = "Bullet")
		float LifeSeconds = 3.0f;

private:
	// �浹�� Sphere
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComp;

	//  �������Ʈ���� �޽� ���� �����ϰ�!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* MeshComp;



	// �߻�ü � ������Ʈ
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* MoveComp;
};
