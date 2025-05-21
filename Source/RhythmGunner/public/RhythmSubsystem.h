// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h" // 반드시 포함

#include "RhythmSubsystem.generated.h"


/*
글로벌하게 박자를 동기화할 방법필요
1. TICK 주기 변경
2. 델리게이트/이벤트 메시지
3. 객체를 생성할때 타이머함수호출하여 박자 카운트
4. 인터페이스 메시지
이벤트가 가장 좋아보임
*/


/*
//사용법
URhythmSubsystem * rhythmsub = GetGameInstance()->GetSubsystem<URhythmSubsystem>();//서브시스템 로드
rhythmsub->OnRhythm().AddUObject(this, &APlayer::Jump);//서브시스템에 함수 등록, 해제는 자동으로 됨
//시스템 박자에 따라 등록된 함수가 자동으로 호출됨 


*/
/*
USTRUCT()//구조체말고 액터로 구현해야하나?
struct F_Note
{
    GENERATED_BODY()

    F_Note(int16 type_, double timing_)
        :type(type_), timing(timing_)
    {
    }

    int16 type=0;
    double timing = 0.f;
};
*/
/*
    박자에 따라 노트 생성 탄약이 다 떨어지는 등의 이유로 인해 존재하는 노트를 수정해야하는경우
    이펙트와 함께 노트 파괴하고 다시 생성?
    순환큐 사용하여 저장
*/

/*
    노트시각화
    카메라를 따라다니는 좌표하나 생성하고
    좌표에 child actor로 노트 생성
*/

UENUM(BlueprintType) // 블루프린트에서도 사용 가능하게
enum class ETimingAcc : uint8
{
    Great      UMETA(DisplayName = "Great"),
    Good     UMETA(DisplayName = "Good"),
    Bad      UMETA(DisplayName = "Bad"),
    Miss      UMETA(DisplayName = "Miss")
};

enum class ENoteDirc : uint8
{
    Left      UMETA(DisplayName = "Left"),
    Right     UMETA(DisplayName = "Right"),
    Up      UMETA(DisplayName = "Up"),
    Down      UMETA(DisplayName = "Down")
};

UCLASS()
class RHYTHMGUNNER_API URhythmSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    // Subsystem overrides
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Tick interface overrides
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override { return !IsTemplate(); }
    virtual TStatId GetStatId() const override;

    virtual bool IsTickableWhenPaused() const override { return false; }
    virtual bool IsTickableInEditor() const override { return false; } //  중요
    virtual UWorld* GetTickableGameObjectWorld() const override { return GWorld; }
    //Assertion failed: !TickableObjects.Contains(TickableObject) 발생
    

    // 예제용 함수
    void PrintTickMessage();
    void RhythmTick(float DeltaTime);
    ETimingAcc CheckNote(ENoteDirc dirc);//출력 great, good, bad, miss 입력 좌우


    DECLARE_EVENT(URhythmSubsystem, FOnRhythm)
    FOnRhythm& OnRhythm() { return RhythmEvent; }


    TDoubleLinkedList<class AFNote*> lNotes;//원형큐는 복사초기화 사용 불가
    TDoubleLinkedList<class AFNote*> rNotes;

    float TickInterval=0.5;
    int16 t = 0;
private:
    FOnRhythm RhythmEvent;
    float TimeSinceLastTick = 0;
    class ANoteAnchor * noteAnchor;

};

