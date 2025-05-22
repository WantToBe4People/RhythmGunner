#include "GameSaveSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void UGameSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    SaveSlotName = TEXT("GameSaveData");
    
    // 저장된 데이터가 없을 때만 초기화
    if (!LoadGameData())
    {
        // 새로운 게임 데이터 초기화
        GameSaveData = NewObject<UGameSaveData>();
        SaveGameData();
    }
}

void UGameSaveSubsystem::Deinitialize()
{
    Super::Deinitialize();
    SaveGameData();
}

bool UGameSaveSubsystem::SaveGameData()
{
    if (GameSaveData)
    {
        return UGameplayStatics::SaveGameToSlot(GameSaveData, SaveSlotName, UserIndex);
    }
    return false;
}

bool UGameSaveSubsystem::LoadGameData()
{
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
    {
        USaveGame* LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex);
        if (LoadedGame)
        {
            GameSaveData = Cast<UGameSaveData>(LoadedGame);
            if (GameSaveData)
            {
                UE_LOG(LogTemp, Log, TEXT("게임 데이터 로드 성공 - 스테이지 데이터 수: %d"), GameSaveData->StageData.Num());
                for (const auto& StagePair : GameSaveData->StageData)
                {
                    UE_LOG(LogTemp, Log, TEXT("스테이지 %d: 클리어 여부: %s, 클리어 시간: %.2f"), 
                        StagePair.Key,
                        StagePair.Value.bIsCleared ? TEXT("클리어") : TEXT("미클리어"),
                        StagePair.Value.ClearTime);
                }
                return true;
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("저장된 게임 데이터가 없습니다."));
    return false;
}

void UGameSaveSubsystem::SetStageClear(int32 StageIndex, float ClearTime)
{
    if (GameSaveData && GameSaveData->StageData.Contains(StageIndex))
    {
        FStageData& StageData = GameSaveData->StageData[StageIndex];
        StageData.bIsCleared = true;
        StageData.ClearTime = ClearTime;
        SaveGameData();
    }
}

bool UGameSaveSubsystem::IsStageCleared(int32 StageIndex) const
{
    if (GameSaveData && GameSaveData->StageData.Contains(StageIndex))
    {
        return GameSaveData->StageData[StageIndex].bIsCleared;
    }
    return false;
}

float UGameSaveSubsystem::GetStageClearTime(int32 StageIndex) const
{
    if (GameSaveData && GameSaveData->StageData.Contains(StageIndex))
    {
        return GameSaveData->StageData[StageIndex].ClearTime;
    }
    return 0.0f;
} 