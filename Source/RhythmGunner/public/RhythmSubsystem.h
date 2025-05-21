// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h" // �ݵ�� ����

#include "RhythmSubsystem.generated.h"


/*
�۷ι��ϰ� ���ڸ� ����ȭ�� ����ʿ�
1. TICK �ֱ� ����
2. ��������Ʈ/�̺�Ʈ �޽���
3. ��ü�� �����Ҷ� Ÿ�̸��Լ�ȣ���Ͽ� ���� ī��Ʈ
4. �������̽� �޽���
�̺�Ʈ�� ���� ���ƺ���
*/


/*
//����
URhythmSubsystem * rhythmsub = GetGameInstance()->GetSubsystem<URhythmSubsystem>();//����ý��� �ε�
rhythmsub->OnRhythm().AddUObject(this, &APlayer::Jump);//����ý��ۿ� �Լ� ���, ������ �ڵ����� ��
//�ý��� ���ڿ� ���� ��ϵ� �Լ��� �ڵ����� ȣ��� 


*/
/*
USTRUCT()//����ü���� ���ͷ� �����ؾ��ϳ�?
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
    ���ڿ� ���� ��Ʈ ���� ź���� �� �������� ���� ������ ���� �����ϴ� ��Ʈ�� �����ؾ��ϴ°��
    ����Ʈ�� �Բ� ��Ʈ �ı��ϰ� �ٽ� ����?
    ��ȯť ����Ͽ� ����
*/

/*
    ��Ʈ�ð�ȭ
    ī�޶� ����ٴϴ� ��ǥ�ϳ� �����ϰ�
    ��ǥ�� child actor�� ��Ʈ ����
*/

UENUM(BlueprintType) // �������Ʈ������ ��� �����ϰ�
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
    virtual bool IsTickableInEditor() const override { return false; } //  �߿�
    virtual UWorld* GetTickableGameObjectWorld() const override { return GWorld; }
    //Assertion failed: !TickableObjects.Contains(TickableObject) �߻�
    

    // ������ �Լ�
    void PrintTickMessage();
    void RhythmTick(float DeltaTime);
    ETimingAcc CheckNote(ENoteDirc dirc);//��� great, good, bad, miss �Է� �¿�


    DECLARE_EVENT(URhythmSubsystem, FOnRhythm)
    FOnRhythm& OnRhythm() { return RhythmEvent; }


    TDoubleLinkedList<class AFNote*> lNotes;//����ť�� �����ʱ�ȭ ��� �Ұ�
    TDoubleLinkedList<class AFNote*> rNotes;

    float TickInterval=0.5;
    int16 t = 0;
private:
    FOnRhythm RhythmEvent;
    float TimeSinceLastTick = 0;
    class ANoteAnchor * noteAnchor;

};

