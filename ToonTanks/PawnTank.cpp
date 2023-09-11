// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"




APawnTank::APawnTank()
{
   SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
   SpringArm->SetupAttachment(RootComponent);

   Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
   Camera->SetupAttachment(SpringArm);

}


void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APawnTank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawnTank::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APawnTank::Fire);

}

void APawnTank::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);

   if(PlayerControllerRef)
   {
      FHitResult HitResult; 
      PlayerControllerRef->GetHitResultUnderCursor(
        ECollisionChannel::ECC_Visibility,
        false,
        HitResult);

       RotateTurret(HitResult.ImpactPoint);

   }
}

void APawnTank::HandleDestruction()
{
   Super::HandleDestruction();
   SetActorHiddenInGame(true);
   SetActorTickEnabled(false);
   bAlive = false;

}


void APawnTank::BeginPlay()
{
   Super::BeginPlay();
   PlayerControllerRef = Cast<APlayerController>(GetController());



}

void APawnTank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed;
    AddActorLocalOffset(DeltaLocation,true);
}

void APawnTank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);     
    AddActorLocalRotation(DeltaRotation, true);
}