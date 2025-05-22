#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RhythmGunnerGameInstance.generated.h"

UCLASS()
class RHYTHMGUNNER_API URhythmGunnerGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;

protected:
    virtual void OnStart() override;

private:
    void SetupDebugInput();
    void OnDebugKeyPressed();
    void ProcessDebugStageClear();

    int32 CurrentDebugStage;
    FTimerHandle DebugTimerHandle;
}; 