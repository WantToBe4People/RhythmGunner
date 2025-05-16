// Fill out your copyright notice in the Description page of Project Settings.


#include "FNote.h"

// Sets default values
AFNote::AFNote()
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
}

// Called when the game starts or when spawned
void AFNote::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFNote::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timing -= DeltaTime;
	SetActorRelativeLocation(FVector(0, 100.f * timing, 0));
	if (timing < -0.01f)
	{
		Destroy();
	}
	//남은 시간에 따라 노트위치 조정 
}

