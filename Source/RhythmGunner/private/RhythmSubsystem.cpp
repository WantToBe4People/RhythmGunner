// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmSubsystem.h"
#include "NoteAnchor.h"
#include "Engine/Engine.h"
#include "Misc/OutputDeviceNull.h"

void URhythmSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    //타이머사용하거나 tickinterval함수만들기
    //GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMySubsystem::TickAtInterval, 1.0f, true);
    
	UWorld* World = GetWorld();
	if (World)
	{
		World->SpawnActor<ANoteAnchor>(ANoteAnchor::StaticClass());
	}
	UE_LOG(LogTemp, Log, TEXT("Subsystem Initialized"));
}

void URhythmSubsystem::Deinitialize()
{
    Super::Deinitialize();
    UE_LOG(LogTemp, Log, TEXT("Subsystem Deinitialized"));
}

void URhythmSubsystem::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Log, TEXT("DT : %f"),DeltaTime);
	if (TickInterval <= 0)
	{
		RhythmTick(DeltaTime);
		return;
	}

	TimeSinceLastTick += DeltaTime;
	while (TimeSinceLastTick > TickInterval)
	{
		RhythmTick(TickInterval);
		TimeSinceLastTick -= TickInterval;
	}
}

void URhythmSubsystem::RhythmTick(float DeltaTime)
{
	UE_LOG(LogTemp, Log, TEXT("RhythmTick : %d"),t);
	++t;
	RhythmEvent.Broadcast();
}

void URhythmSubsystem::PrintTickMessage()
{
    UE_LOG(LogTemp, Log, TEXT("Ticking Subsystem..."));
}

TStatId URhythmSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UMyGameInstanceSubsystem, STATGROUP_Tickables);
}