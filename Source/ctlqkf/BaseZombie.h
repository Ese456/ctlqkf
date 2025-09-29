// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseZombie.generated.h"

UCLASS()
class CTLQKF_API ABaseZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chase")
	AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chase")
	float AcceptanceRadius = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chase")
	float TurnSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chase")
	float ChaseSpeed = 600.f;
};
