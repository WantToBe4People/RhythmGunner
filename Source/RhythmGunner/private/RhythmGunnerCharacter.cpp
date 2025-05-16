// Copyright Epic Games, Inc. All Rights Reserved.

#include "RhythmGunnerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include <Components\WidgetComponent.h>
#include "Blueprint/UserWidget.h"

//////////////////////////////////////////////////////////////////////////
// ARhythmGunnerCharacter

ARhythmGunnerCharacter::ARhythmGunnerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1인칭 카메라
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f)); // 머리 높이

	// 손 또는 총 메시
	/*
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetupAttachment(FirstPersonCameraComponent);
	FP_Gun->SetRelativeLocation(FVector(50.0f, 10.0f, -10.0f)); // 카메라 기준 위치
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	*/


	static ConstructorHelpers::FClassFinder<UUserWidget> CrosshairWidgetClass(TEXT("/Game/UI_BP/WBP_Crosshair.WBP_Crosshair_C")); // 경로는 정확히 맞아야 함
	if (CrosshairWidgetClass.Succeeded())
	{
		crosshairUIFactory = CrosshairWidgetClass.Class;
	}

	if (crosshairUIFactory)
	{
		_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

		if (_crosshairUI)
		{
			UE_LOG(LogTemp, Log, TEXT("크로스헤어 위젯 생성 성공"));
			_crosshairUI->AddToViewport();
		}
	}
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	/// HP 게이지 테스트
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(RootComponent);
	//HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
	//HealthBarWidget->SetDrawSize(FVector2D(150.f, 20.f));
	//HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f)); // 머리 위
	//HealthBarWidget->SetPivot(FVector2D(0.5f, 0.5f));
}

void ARhythmGunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	/// HP UI TEST
	if (HealthBarWidgetClass)
	{
		HealthBarWidget->SetWidgetClass(HealthBarWidgetClass);
		HealthBarWidget->InitWidget();
		HealthBarWidget->SetTwoSided(true);

		float Percent = FMath::Clamp(CurrentHP / MaxHP, 0.f, 1.f);

		if (UUserWidget* Widget = HealthBarWidget->GetUserWidgetObject())
		{
			FString Command = FString::Printf(TEXT("SetHealthPercent %.3f"), Percent);
			Widget->CallFunctionByNameWithArguments(*Command, *GLog, nullptr, true);

			UE_LOG(LogTemp, Warning, TEXT("percent: %f"), Percent);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// Input

void ARhythmGunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARhythmGunnerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARhythmGunnerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ARhythmGunnerCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ARhythmGunnerCharacter::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARhythmGunnerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARhythmGunnerCharacter::StopJump);
}

void ARhythmGunnerCharacter::UpdateHealthUI()
{
	if (CurrentHP <= 0.f)
	{
		return;
	}

	float Percent = FMath::Clamp(CurrentHP / MaxHP, 0.f, 1.f);

	if (UUserWidget* Widget = HealthBarWidget->GetUserWidgetObject())
	{
		FString Command = FString::Printf(TEXT("SetHealthPercent %.3f"), Percent);
		Widget->CallFunctionByNameWithArguments(*Command, *GLog, nullptr, true);

		UE_LOG(LogTemp, Warning, TEXT("percent: %f"), Percent);
	}
}

/// HP UI TEST
void ARhythmGunnerCharacter::ApplyDamage(float Damage)
{
	CurrentHP = FMath::Clamp(CurrentHP - Damage, 0.f, MaxHP);
	UpdateHealthUI();
}

void ARhythmGunnerCharacter::AddHealth(float Amount)
{
	CurrentHP = FMath::Clamp(CurrentHP + Amount, 0.f, MaxHP);
	UpdateHealthUI();
}



void ARhythmGunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	elapsedTime_UI_Test += DeltaTime;
	if (elapsedTime_UI_Test >= 5.f)
	{
		ApplyDamage(10.f);
		elapsedTime_UI_Test = 0.f;

		UE_LOG(LogTemp, Warning, TEXT("HP: %f"), CurrentHP);
	}

	UE_LOG(LogTemp, Warning, TEXT("tick"));
	UE_LOG(LogTemp, Warning, TEXT("I am %s ticking"), *GetName());
}



void ARhythmGunnerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ARhythmGunnerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ARhythmGunnerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ARhythmGunnerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ARhythmGunnerCharacter::StartJump()
{
	bPressedJump = true;
}

void ARhythmGunnerCharacter::StopJump()
{
	bPressedJump = false;
}