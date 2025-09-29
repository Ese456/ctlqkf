// TimerWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

class UTextBlock;
class UProgressBar;
class ATimerActor; // ★ 여기 통일

UCLASS()
class CTLQKF_API UTimerWidget : public UUserWidget
{
    GENERATED_BODY()

public: 
    // 디자이너 트리의 이름이 정확히 "TimerText" 여야 자동 바인딩됨
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* TimerText = nullptr;

    // 선택 위젯: 없으면 null이어도 크래시 안 남
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    UProgressBar* TimerBar = nullptr;

    // 타이머 소스 세팅 (레벨 BP에서 호출)
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void SetTimerSource(ATimerActor* InSource);

protected:
    // BindWidget이 끝난 뒤 호출되므로 UI 접근에 안전
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnTimerUpdated(float CurrentTime);

    UPROPERTY() // GC 안전
        ATimerActor* Source = nullptr;

    FString FormatMMSS(float Seconds) const;
};
