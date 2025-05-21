// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmSubsystem.h"
#include "NoteAnchor.h"
#include "FNote.h"
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
		noteAnchor = World->SpawnActor<ANoteAnchor>(ANoteAnchor::StaticClass());
		
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
	/*
	for (TDoubleLinkedList<F_Note>::TIterator It(lNotes.GetHead()); It; ++It)
	{
		(*It).timing -= DeltaTime;
	}
	for (TDoubleLinkedList<F_Note>::TIterator It(rNotes.GetHead()); It; ++It)
	{
		(*It).timing -= DeltaTime;
	}
	*/



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
	AFNote* fnote = GetWorld()->SpawnActor<AFNote>(AFNote::StaticClass());
	fnote->timing = 2.f;
	fnote->type = 1;
	FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, true);
	fnote->AttachToActor(noteAnchor, Rules);
	lNotes.AddTail(fnote);
	RhythmEvent.Broadcast();
	/*
	*/
	
}

ETimingAcc URhythmSubsystem::CheckNote(ENoteDirc dirc)
{
	AFNote* fnote;

	switch (dirc)
	{
	case ENoteDirc::Left:
		fnote = lNotes.GetHead()->GetValue();
		break;

	case ENoteDirc::Right:
		fnote = rNotes.GetHead()->GetValue();
		break;

	default:
		break;
	}
	return ETimingAcc::Good;
}



void URhythmSubsystem::PrintTickMessage()
{
    UE_LOG(LogTemp, Log, TEXT("Ticking Subsystem..."));
}

TStatId URhythmSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UMyGameInstanceSubsystem, STATGROUP_Tickables);
}