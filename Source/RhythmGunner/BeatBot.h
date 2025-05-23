#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Components/StaticMeshComponent.h"
#include "BeatBot.generated.h"

UCLASS()
class RHYTHMGUNNER_API ABeatBot : public ACharacter
{
	GENERATED_BODY()

public:
	ABeatBot();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// �÷��̾�� �ε����� �� ȣ��
	UFUNCTION()
		void OnOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	// �������Ʈ���� �Ҵ��� ���� ����Ʈ
	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* HitEffect;

	// �̵� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Movement")
		float MoveSpeed = 400.0f;

	// ������ �߰�: �� �ݰ� �̳��� ���� ���� ������
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0"))
		float DetectionRadius = 1000.0f;

private:
	// ���� ��� �÷��̾�
	UPROPERTY()
		ACharacter* Player;

	// ������ �߰�: �ð��� Static Mesh ������
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MeshComp;
};
