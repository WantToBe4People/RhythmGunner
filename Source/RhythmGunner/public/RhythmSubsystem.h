// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h" // �ݵ�� ����
#include "Containers/CircularQueue.h"
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

USTRUCT()
struct F_Note
{
    GENERATED_BODY()



    int16 type=0;
    int16 cor=0;
};
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
    virtual bool IsTickable() const override { return true; }
    virtual TStatId GetStatId() const override;

    // ������ �Լ�
    void PrintTickMessage();
    void RhythmTick(float DeltaTime);
    DECLARE_EVENT(URhythmSubsystem, FOnRhythm)
    FOnRhythm& OnRhythm() { return RhythmEvent; }


    TCircularQueue<F_Note> lNotes{ 6 };//����ť�� �����ʱ�ȭ ��� �Ұ�
    TCircularQueue<F_Note> rNotes{ 6 };

    float TickInterval=0.5;
    int16 t = 0;
private:
    FOnRhythm RhythmEvent;
    float TimeSinceLastTick = 0;
    class ANoteAnchor * noteAnchor;

};

