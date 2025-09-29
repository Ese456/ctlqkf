#include "TimerActor.h"
#include "TimerManager.h"

ATimerActor::ATimerActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATimerActor::BeginPlay()
{
    Super::BeginPlay();

    // ���۰� ����
    if (bCountDown)
    {
        // ī��Ʈ�ٿ��̸� StartTime�� 0�̸� TargetDuration�� �ڵ� ����
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
        // UI �ʱ� ǥ�� ����ȭ
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

    // �̹� ���� ������ �ʱ�ȭ �� �����(�ߺ�����)
    StopTimer();

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &ATimerActor::UpdateTimer,
        TickInterval,
        true  // Loop
    );

    // ��� 1ȸ ����ȭ(���ϸ� ���� ����)
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
    // �ð��� �ʱ�ȭ, ���� ���´� ����
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

    // ���� �ִٸ� ������ �ݿ��Ϸ��� �����
    if (IsRunning())
    {
        const bool bWasPaused = bPaused;
        StartTimer();
        if (bWasPaused) PauseTimer();
    }
}

void ATimerActor::UpdateTimer()
{
    // ƽ �ܰ�
    const float Step = TickInterval;

    if (bCountDown)
    {
        CurrentTime = FMath::Max(0.f, CurrentTime - Step);
    }
    else
    {
        CurrentTime += Step;
    }

    // UI ����
    OnTimerUpdated.Broadcast(CurrentTime);

    // ������: ī��Ʈ���� TargetDuration ����/�ʰ�, ī��Ʈ�ٿ��� 0 ����
    const bool bReachedEnd = bCountDown
        ? (CurrentTime <= 0.f)
        : (TargetDuration > 0.f && CurrentTime >= TargetDuration);

    if (bReachedEnd && !bFinishedBroadcasted)
    {
        bFinishedBroadcasted = true;
        StopTimer();
        OnTimerFinished.Broadcast(); // �� ���� ���
    }
}
