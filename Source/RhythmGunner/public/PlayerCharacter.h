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

    /** �̵� */
    void MoveForward(float Value);
    void MoveRight(float Value);

    /** �þ� ȸ�� */
    void LookUp(float Value);
    void Turn(float Value);

    /** ���� */
    void StartJump();
    void StopJump();

protected:
    /** 1��Ī ī�޶� */
    UPROPERTY(VisibleAnywhere)
        class UCameraComponent* FirstPersonCameraComponent;

    /** 1��Ī�� ���� �޽� �Ǵ� �� */
    UPROPERTY(VisibleDefaultsOnly)
    class USkeletalMeshComponent* FP_Gun;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> crosshairUIFactory;

    class UUserWidget* _crosshairUI;

};
