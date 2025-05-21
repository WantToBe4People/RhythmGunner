// Fill out your copyright notice in the Description page of Project Settings.


#include "FieldItemBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
AFieldItemBase::AFieldItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(80.f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComponent->SetGenerateOverlapEvents(true);
    RootComponent = CollisionComponent;

    IdleEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("IdleEffectComponent"));
    IdleEffectComponent->SetupAttachment(RootComponent);
    IdleEffectComponent->SetAutoActivate(true);
}

// Called when the game starts or when spawned
void AFieldItemBase::BeginPlay()
{
	Super::BeginPlay();
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFieldItemBase::OnOverlapBegin);
}

void AFieldItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        /// 추후 플레이어 클래스로 캐스팅할 것
        ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
        if (PlayerCharacter)
        {
            ApplyItemFunction(PlayerCharacter);

            if (PickupSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
            }

            if (PickupEffect)
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation(), GetActorRotation());
            }

            Destroy();
        }
    }
}

// Called every frame
void AFieldItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

