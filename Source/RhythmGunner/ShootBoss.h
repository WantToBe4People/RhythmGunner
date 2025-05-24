// ShootBoss.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShootBoss.generated.h"

UCLASS()
class RHYTHMGUNNER_API AShootBoss : public ACharacter
{
	GENERATED_BODY()

public:
	AShootBoss();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


private:
	// 총 메시
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		UStaticMeshComponent* GunMesh;

	// 총알 클래스
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<AActor> BulletClass;

	// 발사 주기
	UPROPERTY(EditAnywhere, Category = "Combat")
		float FireInterval = 1.5f;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* ShootMontage;

	float FireTimer = 0.f;

};