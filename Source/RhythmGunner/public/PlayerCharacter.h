// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class RHYTHMGUNNER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
    virtual void BeginPlay() override;

public:
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

};
