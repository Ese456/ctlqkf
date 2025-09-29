// TimerWidget.cpp
#include "TimerWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "TimerActor.h"           // ★ ATimerActor 선언/델리게이트 포함

void UTimerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 초기 화면 값(선택)
    if (TimerText)
    {
        TimerText->SetText(FText::FromString(TEXT("00:00")));
    }
    if (TimerBar)
    {
        TimerBar->SetPercent(0.0f);
    }

    // 이미 Source가 세팅돼 있었다면 1회 동기화
    if (Source)
    {
        OnTimerUpdated(Source->GetCurrentTime());
    }
}

void UTimerWidget::NativeDestruct()
{
    // 델리게이트 해제(누수/중복 바인딩 방지)
    if (Source)
    {
        Source->OnTimerUpdated.RemoveAll(this);
        Source = nullptr;
    }
    Super::NativeDestruct();
}

void UTimerWidget::SetTimerSource(ATimerActor* InSource)
{
    // 기존 소스 해제
    if (Source)
    {
        Source->OnTimerUpdated.RemoveAll(this);
    }

    Source = InSource;

    if (Source)
    {
        // 타이머 업데이트 델리게이트 구독
        Source->OnTimerUpdated.AddDynamic(this, &UTimerWidget::OnTimerUpdated);

        // 즉시 1회 갱신
        OnTimerUpdated(Source->GetCurrentTime());
    }
    else
    {
        // 소스가 사라졌을 때 안전한 기본 상태
        if (TimerText) TimerText->SetText(FText::FromString(TEXT("00:00")));
        if (TimerBar)  TimerBar->SetPercent(0.0f);
    }
}

void UTimerWidget::OnTimerUpdated(float CurrentTime)
{
    if (TimerText)
    {
        TimerText->SetText(FText::FromString(FormatMMSS(CurrentTime)));
    }

    if (TimerBar && Source && Source->TargetDuration > 0.f)
    {
        const float Percent = FMath::Clamp(CurrentTime / Source->TargetDuration, 0.f, 1.f);
        TimerBar->SetPercent(Percent);
    }
}

FString UTimerWidget::FormatMMSS(float Seconds) const
{
    const int32 Total = FMath::Max(0, FMath::FloorToInt(Seconds));
    const int32 M = Total / 60;
    const int32 S = Total % 60;
    return FString::Printf(TEXT("%d:%02d"), M, S);
}
