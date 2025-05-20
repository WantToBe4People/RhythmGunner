// Copyright Epic Games, Inc. All Rights Reserved.

#include "RhythmGunnerCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <Components\WidgetComponent.h>
#include "Blueprint/UserWidget.h"

//////////////////////////////////////////////////////////////////////////
// ARhythmGunnerCharacter

ARhythmGunnerCharacter::ARhythmGunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	/// HP 게이지 테슽트
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(RootComponent);
	//HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
	//HealthBarWidget->SetDrawSize(FVector2D(150.f, 20.f));
	//HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f)); // 머리 위
	//HealthBarWidget->SetPivot(FVector2D(0.5f, 0.5f));
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARhythmGunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARhythmGunnerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARhythmGunnerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARhythmGunnerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARhythmGunnerCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARhythmGunnerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARhythmGunnerCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ARhythmGunnerCharacter::OnResetVR);

}

/// HP UI TEST
void ARhythmGunnerCharacter::ApplyDamage(float Damage)
{
	if (CurrentHP <= 0.f)
	{
		float Percent = 0.f;
		if (UUserWidget* Widget = HealthBarWidget->GetUserWidgetObject())
		{
			FString Command = FString::Printf(TEXT("SetHealthPercent %.3f"), Percent);
			Widget->CallFunctionByNameWithArguments(*Command, *GLog, nullptr, true);
		}
		return;
	}

	CurrentHP = FMath::Clamp(CurrentHP - Damage, 0.f, MaxHP);
	float Percent = FMath::Clamp(CurrentHP / MaxHP, 0.f, 1.f);

	if (UUserWidget* Widget = HealthBarWidget->GetUserWidgetObject())
	{
		FString Command = FString::Printf(TEXT("SetHealthPercent %.3f"), Percent);
		Widget->CallFunctionByNameWithArguments(*Command, *GLog, nullptr, true);

		UE_LOG(LogTemp, Warning, TEXT("percent: %f"), Percent);
	}
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

void ARhythmGunnerCharacter::OnResetVR()
{
	// If RhythmGunner is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in RhythmGunner.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ARhythmGunnerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ARhythmGunnerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ARhythmGunnerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARhythmGunnerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARhythmGunnerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARhythmGunnerCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
