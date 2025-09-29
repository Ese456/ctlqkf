// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseZombie.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
ABaseZombie::ABaseZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseZombie::BeginPlay()
{
	Super::BeginPlay();
    TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	
}

// Called every frame
void ABaseZombie::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!IsValid(TargetActor)) return;

    const FVector MyLoc = GetActorLocation();
    const FVector TargetLoc = TargetActor->GetActorLocation();
    const FVector ToTarget = TargetLoc - MyLoc;
    const float Dist = ToTarget.Length();

    if (Dist <= AcceptanceRadius) return;

    // 회전 보간
    const FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(MyLoc, TargetLoc);
    const FRotator NewRot = FMath::RInterpTo(GetActorRotation(), LookRot, DeltaSeconds, TurnSpeed);
    SetActorRotation(NewRot);

    // 이동
    const FVector Dir = ToTarget.GetSafeNormal();
    AddMovementInput(Dir, 1.0f);
}
