// TimerWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

class UTextBlock;
class UProgressBar;
class ATimerActor; // �� ���� ����

UCLASS()
class CTLQKF_API UTimerWidget : public UUserWidget
{
    GENERATED_BODY()

public: 
    // �����̳� Ʈ���� �̸��� ��Ȯ�� "TimerText" ���� �ڵ� ���ε���
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* TimerText = nullptr;

    // ���� ����: ������ null�̾ ũ���� �� ��
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    UProgressBar* TimerBar = nullptr;

    // Ÿ�̸� �ҽ� ���� (���� BP���� ȣ��)
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void SetTimerSource(ATimerActor* InSource);

protected:
    // BindWidget�� ���� �� ȣ��ǹǷ� UI ���ٿ� ����
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnTimerUpdated(float CurrentTime);

    UPROPERTY() // GC ����
        ATimerActor* Source = nullptr;

    FString FormatMMSS(float Seconds) const;
};
