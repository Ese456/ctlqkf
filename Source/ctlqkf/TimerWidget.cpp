// TimerWidget.cpp
#include "TimerWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "TimerActor.h"           // �� ATimerActor ����/��������Ʈ ����

void UTimerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // �ʱ� ȭ�� ��(����)
    if (TimerText)
    {
        TimerText->SetText(FText::FromString(TEXT("00:00")));
    }
    if (TimerBar)
    {
        TimerBar->SetPercent(0.0f);
    }

    // �̹� Source�� ���õ� �־��ٸ� 1ȸ ����ȭ
    if (Source)
    {
        OnTimerUpdated(Source->GetCurrentTime());
    }
}

void UTimerWidget::NativeDestruct()
{
    // ��������Ʈ ����(����/�ߺ� ���ε� ����)
    if (Source)
    {
        Source->OnTimerUpdated.RemoveAll(this);
        Source = nullptr;
    }
    Super::NativeDestruct();
}

void UTimerWidget::SetTimerSource(ATimerActor* InSource)
{
    // ���� �ҽ� ����
    if (Source)
    {
        Source->OnTimerUpdated.RemoveAll(this);
    }

    Source = InSource;

    if (Source)
    {
        // Ÿ�̸� ������Ʈ ��������Ʈ ����
        Source->OnTimerUpdated.AddDynamic(this, &UTimerWidget::OnTimerUpdated);

        // ��� 1ȸ ����
        OnTimerUpdated(Source->GetCurrentTime());
    }
    else
    {
        // �ҽ��� ������� �� ������ �⺻ ����
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
