#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerActor.generated.h"

// 매 틱 업데이트 (현재 시간 전달)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerUpdated, float, CurrentTime);
// 목표 도달(끝) 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimerFinished);

UCLASS()
class CTLQKF_API ATimerActor : public AActor
{
    GENERATED_BODY()

public:
    ATimerActor();

    /** 매 업데이트 브로드캐스트 (UI 갱신에 사용) */
    UPROPERTY(BlueprintAssignable, Category = "Timer")
    FTimerUpdated OnTimerUpdated;

    /** 타이머가 목표에 도달했을 때 한 번 브로드캐스트 */
    UPROPERTY(BlueprintAssignable, Category = "Timer")
    FTimerFinished OnTimerFinished;

    /** 진행바 분모로 쓰일 목표 시간(초). CountUp이면 도달 기준, CountDown이면 0 도달 기준 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (ClampMin = "0.0"))
    float TargetDuration = 60.f;

    /** 레벨 시작 시 자동으로 StartTimer 호출 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    bool bAutoStart = true;

    /** true면 카운트다운(예: 60→0), false면 카운트업(0→목표) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    bool bCountDown = false;

    /** 시작 시간 값 (카운트다운이면 보통 TargetDuration과 동일하게 셋업) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (ClampMin = "0.0"))
    float StartTime = 0.f;

    /** 타이머 틱 간격(초). UI는 이 간격마다 갱신됨 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (ClampMin = "0.01"))
    float TickInterval = 1.0f;

    /** 현재 시간(초) 읽기 */
    UFUNCTION(BlueprintCallable, Category = "Timer")
    float GetCurrentTime() const { return CurrentTime; }

    /** 타이머 시작/정지/일시정지/재개 */
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void StartTimer();
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void StopTimer();
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void PauseTimer();
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void ResumeTimer();

    /** 값/간격 설정 유틸 */
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void ResetTimer();
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void SetCurrentTime(float NewTime);
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void SetTickInterval(float NewInterval);

    /** 실행 중인지 여부 */
    UFUNCTION(BlueprintPure, Category = "Timer")
    bool IsRunning() const;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    void UpdateTimer();

    /** 내부 타이머 핸들 */
    FTimerHandle TimerHandle;

    /** 내부 현재 시간 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer", meta = (AllowPrivateAccess = "true"))
    float CurrentTime = 0.f;

    /** 일시정지 상태 여부 */
    bool bPaused = false;

    /** 목표 도달 브로드캐스트가 중복되지 않게 제어 */
    bool bFinishedBroadcasted = false;
};
