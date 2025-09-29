#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerActor.generated.h"

// �� ƽ ������Ʈ (���� �ð� ����)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerUpdated, float, CurrentTime);
// ��ǥ ����(��) �̺�Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimerFinished);

UCLASS()
class CTLQKF_API ATimerActor : public AActor
{
    GENERATED_BODY()

public:
    ATimerActor();

    /** �� ������Ʈ ��ε�ĳ��Ʈ (UI ���ſ� ���) */
    UPROPERTY(BlueprintAssignable, Category = "Timer")
    FTimerUpdated OnTimerUpdated;

    /** Ÿ�̸Ӱ� ��ǥ�� �������� �� �� �� ��ε�ĳ��Ʈ */
    UPROPERTY(BlueprintAssignable, Category = "Timer")
    FTimerFinished OnTimerFinished;

    /** ����� �и�� ���� ��ǥ �ð�(��). CountUp�̸� ���� ����, CountDown�̸� 0 ���� ���� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (ClampMin = "0.0"))
    float TargetDuration = 60.f;

    /** ���� ���� �� �ڵ����� StartTimer ȣ�� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    bool bAutoStart = true;

    /** true�� ī��Ʈ�ٿ�(��: 60��0), false�� ī��Ʈ��(0���ǥ) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    bool bCountDown = false;

    /** ���� �ð� �� (ī��Ʈ�ٿ��̸� ���� TargetDuration�� �����ϰ� �¾�) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (ClampMin = "0.0"))
    float StartTime = 0.f;

    /** Ÿ�̸� ƽ ����(��). UI�� �� ���ݸ��� ���ŵ� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (ClampMin = "0.01"))
    float TickInterval = 1.0f;

    /** ���� �ð�(��) �б� */
    UFUNCTION(BlueprintCallable, Category = "Timer")
    float GetCurrentTime() const { return CurrentTime; }

    /** Ÿ�̸� ����/����/�Ͻ�����/�簳 */
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void StartTimer();
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void StopTimer();
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void PauseTimer();
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void ResumeTimer();

    /** ��/���� ���� ��ƿ */
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void ResetTimer();
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void SetCurrentTime(float NewTime);
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void SetTickInterval(float NewInterval);

    /** ���� ������ ���� */
    UFUNCTION(BlueprintPure, Category = "Timer")
    bool IsRunning() const;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    void UpdateTimer();

    /** ���� Ÿ�̸� �ڵ� */
    FTimerHandle TimerHandle;

    /** ���� ���� �ð� */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer", meta = (AllowPrivateAccess = "true"))
    float CurrentTime = 0.f;

    /** �Ͻ����� ���� ���� */
    bool bPaused = false;

    /** ��ǥ ���� ��ε�ĳ��Ʈ�� �ߺ����� �ʰ� ���� */
    bool bFinishedBroadcasted = false;
};
