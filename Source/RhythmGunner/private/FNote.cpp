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

	// ���Ǿ� �޽� ���� �ε� (���� �⺻ �޽� ���)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
	}

	// �浹 ��Ȱ��ȭ
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereMesh->SetGenerateOverlapEvents(false);

	SphereMesh->CastShadow = false;              // �׸��� ���� ��Ȱ��ȭ
	SphereMesh->bCastDynamicShadow = false;      // ���� �׸��� ��Ȱ��ȭ
	SphereMesh->bCastStaticShadow = false;       // ���� �׸��� ��Ȱ��ȭ
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
	//���� �ð��� ���� ��Ʈ��ġ ���� 
}

