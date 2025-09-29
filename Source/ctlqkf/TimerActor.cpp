#include "TimerActor.h"
#include "TimerManager.h"

ATimerActor::ATimerActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATimerActor::BeginPlay()
{
    Super::BeginPlay();

    // 시작값 세팅
    if (bCountDown)
    {
        // 카운트다운이면 StartTime이 0이면 TargetDuration로 자동 설정
        CurrentTime = (StartTime > 0.f) ? StartTime : TargetDuration;
    }
    else
    {
        CurrentTime = StartTime;
    }

    bFinishedBroadcasted = false;
    bPaused = false;

    if (bAutoStart)
    {
        StartTimer();
    }
    else
    {
        // UI 초기 표시 동기화
        OnTimerUpdated.Broadcast(CurrentTime);
    }
}

void ATimerActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    StopTimer();
    Super::EndPlay(EndPlayReason);
}

bool ATimerActor::IsRunning() const
{
    return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(TimerHandle) && !bPaused;
}

void ATimerActor::StartTimer()
{
    if (!GetWorld()) return;

    bPaused = false;
    bFinishedBroadcasted = false;

    // 이미 돌고 있으면 초기화 후 재시작(중복방지)
    StopTimer();

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &ATimerActor::UpdateTimer,
        TickInterval,
        true  // Loop
    );

    // 즉시 1회 동기화(원하면 제거 가능)
    OnTimerUpdated.Broadcast(CurrentTime);
}

void ATimerActor::StopTimer()
{
    if (!GetWorld()) return;
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    bPaused = false;
}

void ATimerActor::PauseTimer()
{
    if (!GetWorld()) return;
    if (IsRunning())
    {
        GetWorld()->GetTimerManager().PauseTimer(TimerHandle);
        bPaused = true;
    }
}

void ATimerActor::ResumeTimer()
{
    if (!GetWorld()) return;
    if (bPaused)
    {
        GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle);
        bPaused = false;
    }
}

void ATimerActor::ResetTimer()
{
    // 시간만 초기화, 실행 상태는 유지
    if (bCountDown)
    {
        CurrentTime = (StartTime > 0.f) ? StartTime : TargetDuration;
    }
    else
    {
        CurrentTime = StartTime;
    }
    bFinishedBroadcasted = false;
    OnTimerUpdated.Broadcast(CurrentTime);
}

void ATimerActor::SetCurrentTime(float NewTime)
{
    CurrentTime = FMath::Max(0.f, NewTime);
    bFinishedBroadcasted = false;
    OnTimerUpdated.Broadcast(CurrentTime);
}

void ATimerActor::SetTickInterval(float NewInterval)
{
    TickInterval = FMath::Max(0.01f, NewInterval);

    // 돌고 있다면 간격을 반영하려면 재시작
    if (IsRunning())
    {
        const bool bWasPaused = bPaused;
        StartTimer();
        if (bWasPaused) PauseTimer();
    }
}

void ATimerActor::UpdateTimer()
{
    // 틱 단계
    const float Step = TickInterval;

    if (bCountDown)
    {
        CurrentTime = FMath::Max(0.f, CurrentTime - Step);
    }
    else
    {
        CurrentTime += Step;
    }

    // UI 갱신
    OnTimerUpdated.Broadcast(CurrentTime);

    // 끝조건: 카운트업은 TargetDuration 도달/초과, 카운트다운은 0 도달
    const bool bReachedEnd = bCountDown
        ? (CurrentTime <= 0.f)
        : (TargetDuration > 0.f && CurrentTime >= TargetDuration);

    if (bReachedEnd && !bFinishedBroadcasted)
    {
        bFinishedBroadcasted = true;
        StopTimer();
        OnTimerFinished.Broadcast(); // 한 번만 방송
    }
}
