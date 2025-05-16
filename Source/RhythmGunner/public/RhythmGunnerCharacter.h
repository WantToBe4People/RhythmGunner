// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RhythmGunnerCharacter.generated.h"


class UWidgetComponent;

UCLASS(config=Game)
class ARhythmGunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	

public:
	ARhythmGunnerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 이동 */
	void MoveForward(float Value);
	void MoveRight(float Value);

	/** 시야 회전 */
	void LookUp(float Value);
	void Turn(float Value);

	/** 점프 */
	void StartJump();
	void StopJump();

protected:
	/** 1인칭 카메라 */
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FirstPersonCameraComponent;

	/** 1인칭용 무기 메시 또는 손 */
	UPROPERTY(VisibleDefaultsOnly)
	class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	class UUserWidget* _crosshairUI;

private:
	/// HP UI TEST
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	UWidgetComponent* HealthBarWidget;

	float MaxHP = 100.f;
	float CurrentHP = 100.f;

	void UpdateHealthUI();

	float elapsedTime_UI_Test = 0.f;

public:
	void ApplyDamage(float Damage);
	void AddHealth(float Amount);
	void AddPlayerMAXHP(float MaxVal) { MaxHP += MaxVal; }
	const float GetPlayerCurrentHP() { return CurrentHP; }
	const float GetPlayerMaxHP() { return MaxHP; }
	///
};

