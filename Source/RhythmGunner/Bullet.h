#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class RHYTHMGUNNER_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// 생성자
	ABullet();

protected:
	// 스폰 직후
	virtual void BeginPlay() override;

	// 충돌 시 호출
	UFUNCTION()
		void OnHit(
			UPrimitiveComponent* HitComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit
		);

public:
	// 매 프레임은 필요 없으면 꺼두셔도 됩니다
	virtual void Tick(float DeltaTime) override;

	// 데미지 값 (블루프린트 편집 가능)
	UPROPERTY(EditAnywhere, Category = "Bullet")
		float Damage = 10.0f;

	// (Optional) 생존 시간 초과 시 자동 파괴
	UPROPERTY(EditAnywhere, Category = "Bullet")
		float LifeSeconds = 3.0f;

private:
	// 충돌용 Sphere
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* CollisionComp;

	//  블루프린트에서 메쉬 변경 가능하게!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* MeshComp;



	// 발사체 운동 컴포넌트
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* MoveComp;
};
