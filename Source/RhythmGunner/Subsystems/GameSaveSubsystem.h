#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameFramework/SaveGame.h"
#include "GameSaveSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FStageData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Stage Data")
    bool bIsCleared;

    UPROPERTY(BlueprintReadWrite, Category = "Stage Data")
    float ClearTime;

    FStageData()
        : bIsCleared(false)
        , ClearTime(0.0f)
    {
    }
};

UCLASS()
class RHYTHMGUNNER_API UGameSaveData : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "Save Data")
    TMap<int32, FStageData> StageData;

    UGameSaveData()
    {
        // 스테이지 1~3 초기화
        for (int32 StageIndex = 1; StageIndex <= 3; ++StageIndex)
        {
            StageData.Add(StageIndex, FStageData());
        }
    }
};

UCLASS()
class RHYTHMGUNNER_API UGameSaveSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Save System")
    bool SaveGameData();

    UFUNCTION(BlueprintCallable, Category = "Save System")
    bool LoadGameData();

    UFUNCTION(BlueprintCallable, Category = "Save System")
    UGameSaveData* GetGameSaveData() const { return GameSaveData; }

    UFUNCTION(BlueprintCallable, Category = "Save System")
    void SetGameSaveData(UGameSaveData* NewData) { GameSaveData = NewData; }

    UFUNCTION(BlueprintCallable, Category = "Save System")
    void SetStageClear(int32 StageIndex, float ClearTime);

    UFUNCTION(BlueprintCallable, Category = "Save System")
    bool IsStageCleared(int32 StageIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Save System")
    float GetStageClearTime(int32 StageIndex) const;

private:
    UPROPERTY()
    UGameSaveData* GameSaveData;
    FString SaveSlotName;
    const int32 UserIndex = 0;
}; 