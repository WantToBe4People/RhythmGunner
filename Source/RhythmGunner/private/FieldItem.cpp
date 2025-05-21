// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldItem.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h" // or your custom character class
#include "Kismet/GameplayStatics.h"

// Sets default values
AFieldItem::AFieldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(100.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = CollisionComponent;

    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
    NiagaraComponent->SetupAttachment(RootComponent);
    NiagaraComponent->SetAutoActivate(true);
}

// Called when the game starts or when spawned
void AFieldItem::BeginPlay()
{
	Super::BeginPlay();
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFieldItem::OnOverlapBegin);
}

void AFieldItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
        if (PlayerCharacter)
        {
            ApplyItemFunction(PlayerCharacter);
            // 예: 체력 회복 기능 추가 부분
            // 만약 커스텀 캐릭터 클래스에서 체력 시스템 구현했다면 Cast 후 체력 증가 처리
            // 예: Cast<AYourCharacter>(PlayerCharacter)->AddHealth(HealAmount);

            // 임시: 사운드나 파티클 이펙트 재생 가능

            // 이펙트 종료 후 액터 제거
            // 이펙트 재생
            if (PickupEffect)
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    this,
                    PickupEffect,
                    GetActorLocation(),
                    GetActorRotation()
                );
            }

            Destroy();
        }
    }
}

// Called every frame
void AFieldItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

