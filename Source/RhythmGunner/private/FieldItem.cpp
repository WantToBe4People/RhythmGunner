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
            // ��: ü�� ȸ�� ��� �߰� �κ�
            // ���� Ŀ���� ĳ���� Ŭ�������� ü�� �ý��� �����ߴٸ� Cast �� ü�� ���� ó��
            // ��: Cast<AYourCharacter>(PlayerCharacter)->AddHealth(HealAmount);

            // �ӽ�: ���峪 ��ƼŬ ����Ʈ ��� ����

            // ����Ʈ ���� �� ���� ����
            // ����Ʈ ���
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

