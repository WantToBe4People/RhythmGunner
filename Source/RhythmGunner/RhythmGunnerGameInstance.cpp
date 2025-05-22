/**
 * @brief 게임 저장/로드 시스템
 * 
 * 이 게임은 UGameSaveSubsystem을 통해 게임 데이터를 관리합니다.
 * 주요 기능:
 * 1. 스테이지 클리어 상태 저장
 * 2. 게임 진행도 저장
 * 3. 디버그 기능 (Ctrl + 9)을 통한 스테이지 클리어 테스트
 * 
 * 저장 데이터 구조:
 * - 스테이지별 클리어 상태
 * - 스테이지별 클리어 시간
 * 
 * 디버그 기능:
 * - Ctrl + 9: 현재 스테이지 클리어
 * - 모든 스테이지가 클리어된 상태에서 Ctrl + 9: 모든 스테이지 초기화
 * 
 * 사용 방법:
 * 1. 게임 저장하기:
 *    UGameSaveSubsystem* SaveSubsystem = GetSubsystem<UGameSaveSubsystem>();
 *    SaveSubsystem->SetStageClear(StageIndex, ClearTime);
 * 
 * 2. 게임 로드하기:
 *    UGameSaveSubsystem* SaveSubsystem = GetSubsystem<UGameSaveSubsystem>();
 *    bool bIsCleared = SaveSubsystem->IsStageCleared(StageIndex);
 * 
 * 3. 모든 데이터 초기화:
 *    UGameSaveSubsystem* SaveSubsystem = GetSubsystem<UGameSaveSubsystem>();
 *    SaveSubsystem->SetGameSaveData(NewObject<UGameSaveData>());
 */

#include "RhythmGunnerGameInstance.h"
#include "Subsystems/GameSaveSubsystem.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/InputSettings.h"

void URhythmGunnerGameInstance::Init()
{
    UE_LOG(LogTemp, Log, TEXT("디버그: GameInstance Init 시작 - 클래스: %s"), *GetClass()->GetName());
    Super::Init();
    
    // 서브시스템 초기화
    if (UGameSaveSubsystem* SaveSubsystem = GetSubsystem<UGameSaveSubsystem>())
    {
        UE_LOG(LogTemp, Log, TEXT("디버그: GameSaveSubsystem 초기화 성공"));
        UE_LOG(LogTemp, Log, TEXT("디버그: GameSaveSubsystem 주소: %p"), SaveSubsystem);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("디버그: GameSaveSubsystem 초기화 실패"));
    }
    
    CurrentDebugStage = 1;
    UE_LOG(LogTemp, Log, TEXT("디버그: GameInstance Init 완료"));
    
    // 입력 설정을 여기서는 하지 않음
    UE_LOG(LogTemp, Log, TEXT("디버그: 현재 GameInstance 클래스: %s"), *GetClass()->GetName());
}

void URhythmGunnerGameInstance::Shutdown()
{
    Super::Shutdown();
    if (DebugTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(DebugTimerHandle);
    }
}

void URhythmGunnerGameInstance::OnStart()
{
    UE_LOG(LogTemp, Log, TEXT("디버그: GameInstance OnStart 시작 - 클래스: %s"), *GetClass()->GetName());
    Super::OnStart();
    
    // 입력 설정
    SetupDebugInput();
    
    // 입력 설정이 제대로 되었는지 확인
    if (APlayerController* PC = GetFirstLocalPlayerController())
    {
        UE_LOG(LogTemp, Log, TEXT("디버그: PlayerController 클래스: %s"), *PC->GetClass()->GetName());
        
        if (PC->InputComponent)
        {
            UE_LOG(LogTemp, Log, TEXT("디버그: InputComponent 확인됨"));
            // 입력 설정 확인
            if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
            {
                TArray<FInputActionKeyMapping> ActionMappings;
                InputSettings->GetActionMappingByName("DebugStageClear", ActionMappings);
                if (ActionMappings.Num() > 0)
                {
                    UE_LOG(LogTemp, Log, TEXT("디버그: DebugStageClear 액션 매핑 확인됨 - 수: %d"), ActionMappings.Num());
                    for (const FInputActionKeyMapping& Mapping : ActionMappings)
                    {
                        UE_LOG(LogTemp, Log, TEXT("디버그: 매핑 - Key: %s, Ctrl: %d"), *Mapping.Key.ToString(), Mapping.bCtrl);
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("디버그: DebugStageClear 액션 매핑이 없습니다!"));
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("디버그: InputComponent가 없습니다!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("디버그: PlayerController를 가져올 수 없습니다!"));
        // PlayerController가 생성될 때까지 대기
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().SetTimerForNextTick(this, &URhythmGunnerGameInstance::OnStart);
            UE_LOG(LogTemp, Log, TEXT("디버그: 다음 틱에서 다시 시도"));
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("디버그: GameInstance OnStart 완료"));
}

void URhythmGunnerGameInstance::SetupDebugInput()
{
    UE_LOG(LogTemp, Log, TEXT("디버그: SetupDebugInput 시작"));
    
    if (UInputSettings* InputSettings = UInputSettings::GetInputSettings())
    {
        UE_LOG(LogTemp, Log, TEXT("디버그: InputSettings 가져오기 성공"));
        
        // 기존 바인딩 제거
        InputSettings->RemoveActionMapping(FInputActionKeyMapping("DebugStageClear", EKeys::Nine, true, false, false, false));
        UE_LOG(LogTemp, Log, TEXT("디버그: 기존 바인딩 제거 완료"));
        
        // 새로운 바인딩 추가 (Ctrl + 9)
        FInputActionKeyMapping NewMapping("DebugStageClear", EKeys::Nine);
        NewMapping.bCtrl = true;
        InputSettings->AddActionMapping(NewMapping);
        UE_LOG(LogTemp, Log, TEXT("디버그: 새로운 바인딩 추가 완료"));
        
        // 입력 설정 저장
        InputSettings->SaveKeyMappings();
        InputSettings->ForceRebuildKeymaps();
        UE_LOG(LogTemp, Log, TEXT("디버그: 입력 설정 저장 및 재구성 완료"));
        
        // 입력 이벤트 바인딩
        if (APlayerController* PC = GetFirstLocalPlayerController())
        {
            UE_LOG(LogTemp, Log, TEXT("디버그: PlayerController 가져오기 성공"));
            
            if (!PC->InputComponent)
            {
                PC->InputComponent = NewObject<UInputComponent>(PC);
                PC->InputComponent->RegisterComponent();
            }
            
            PC->InputComponent->BindAction("DebugStageClear", IE_Pressed, this, &URhythmGunnerGameInstance::OnDebugKeyPressed);
            UE_LOG(LogTemp, Log, TEXT("디버그: 입력 이벤트 바인딩 완료"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("디버그: PlayerController를 가져올 수 없습니다!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("디버그: InputSettings를 가져올 수 없습니다!"));
    }
}

void URhythmGunnerGameInstance::OnDebugKeyPressed()
{
    UE_LOG(LogTemp, Log, TEXT("디버그: OnDebugKeyPressed 호출됨"));
    ProcessDebugStageClear();
}

void URhythmGunnerGameInstance::ProcessDebugStageClear()
{
    UE_LOG(LogTemp, Log, TEXT("디버그: ProcessDebugStageClear 시작"));
    
    if (UGameSaveSubsystem* SaveSubsystem = GetSubsystem<UGameSaveSubsystem>())
    {
        UE_LOG(LogTemp, Log, TEXT("디버그: SaveSubsystem 가져오기 성공"));
        
        // 모든 스테이지가 클리어되었는지 확인
        bool bAllStagesCleared = true;
        for (int32 StageIndex = 1; StageIndex <= 3; ++StageIndex)
        {
            if (!SaveSubsystem->IsStageCleared(StageIndex))
            {
                bAllStagesCleared = false;
                break;
            }
        }

        if (bAllStagesCleared)
        {
            // 모든 스테이지가 클리어되었으면 초기화
            SaveSubsystem->SetGameSaveData(NewObject<UGameSaveData>());
            CurrentDebugStage = 1;
            UE_LOG(LogTemp, Log, TEXT("디버그: 모든 스테이지 초기화 완료"));
        }
        else
        {
            // 현재 스테이지 클리어
            SaveSubsystem->SetStageClear(CurrentDebugStage, 60.0f);
            UE_LOG(LogTemp, Log, TEXT("디버그: 스테이지 %d 클리어 완료"), CurrentDebugStage);
            CurrentDebugStage++;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("디버그: SaveSubsystem을 가져올 수 없습니다!"));
    }
} 