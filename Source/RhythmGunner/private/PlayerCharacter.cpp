// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // 1��Ī ī�޶�
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(RootComponent);
    FirstPersonCameraComponent->bUsePawnControlRotation = true;
    FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f)); // �Ӹ� ����

    // �� �Ǵ� �� �޽�
    /*
    FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
    FP_Gun->SetupAttachment(FirstPersonCameraComponent);
    FP_Gun->SetRelativeLocation(FVector(50.0f, 10.0f, -10.0f)); // ī�޶� ���� ��ġ
    FP_Gun->bCastDynamicShadow = false;
    FP_Gun->CastShadow = false;
    */


    static ConstructorHelpers::FClassFinder<UUserWidget> CrosshairWidgetClass(TEXT("/Game/BluePrints/WBP_Crosshair")); // ��δ� ��Ȯ�� �¾ƾ� ��
    if (CrosshairWidgetClass.Succeeded())
    {
        crosshairUIFactory = CrosshairWidgetClass.Class;
    }

    if (crosshairUIFactory)
    {
        _crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
        
        if (_crosshairUI)
        {
            UE_LOG(LogTemp, Log, TEXT("ũ�ν���� ���� ���� ����"));
            _crosshairUI->AddToViewport();
        }
    }
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
    PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);
}

void APlayerCharacter::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void APlayerCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Value);
    }
}

void APlayerCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}

void APlayerCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void APlayerCharacter::StartJump()
{
    bPressedJump = true;
}

void APlayerCharacter::StopJump()
{
    bPressedJump = false;
}
