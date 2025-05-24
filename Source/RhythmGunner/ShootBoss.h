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
	// �� �޽�
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		UStaticMeshComponent* GunMesh;

	// �Ѿ� Ŭ����
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<AActor> BulletClass;

	// �߻� �ֱ�
	UPROPERTY(EditAnywhere, Category = "Combat")
		float FireInterval = 1.5f;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* ShootMontage;

	float FireTimer = 0.f;

};