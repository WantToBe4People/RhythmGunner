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
	SphereMesh->SetWorldScale3D(FVector(0.1f));
	SphereMesh->SetupAttachment(RootComponent);

	// 스피어 메시 에셋 로드 (엔진 기본 메시 사용)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
	}

	// 충돌 비활성화
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereMesh->SetGenerateOverlapEvents(false);

	SphereMesh->CastShadow = false;              // 그림자 생성 비활성화
	SphereMesh->bCastDynamicShadow = false;      // 동적 그림자 비활성화
	SphereMesh->bCastStaticShadow = false;       // 정적 그림자 비활성화

	SphereMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void ANoteAnchor::BeginPlay()
{
	Super::BeginPlay();
	// 플레이어 컨트롤러에서 카메라 매니저 가져오기
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PlayerCameraManager = PC->PlayerCameraManager;
	}
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;//카메라 댐핑 방지
}

// Called every frame
void ANoteAnchor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCameraManager)
	{
		// 카메라 위치와 앞 방향 가져오기
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
		FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();
		FVector ForwardVector = CameraRotation.Vector(); // 또는 GetForwardVector()
		FVector DownVector = -FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Z); // 아래 방향

		// 카메라 앞 300유닛 위치로 이동
		FVector TargetLocation = CameraLocation + ForwardVector * 100.0f + DownVector * 0.f;//댐핑 없애야함
		//FVector TargetLocation = CameraLocation + ForwardVector * 100.0f;
		SetActorLocation(TargetLocation);
		SetActorRotation(CameraRotation);
	}
}

