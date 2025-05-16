// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteAnchor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ANoteAnchor::ANoteAnchor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);

	// ���Ǿ� �޽� ���� �ε� (���� �⺻ �޽� ���)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
	}

	// �浹 ��Ȱ��ȭ
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereMesh->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void ANoteAnchor::BeginPlay()
{
	Super::BeginPlay();
	// �÷��̾� ��Ʈ�ѷ����� ī�޶� �Ŵ��� ��������
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PlayerCameraManager = PC->PlayerCameraManager;
	}
	
}

// Called every frame
void ANoteAnchor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCameraManager)
	{
		// ī�޶� ��ġ�� �� ���� ��������
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
		FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();
		FVector ForwardVector = CameraRotation.Vector(); // �Ǵ� GetForwardVector()
		FVector DownVector = -FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Z); // �Ʒ� ����

		// ī�޶� �� 300���� ��ġ�� �̵�
		FVector TargetLocation = CameraLocation + ForwardVector * 600.0f + DownVector * 100.f;
		SetActorLocation(TargetLocation);
		SetActorRotation(CameraRotation);
	}
}

