// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "TriggerableActor.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ATriggerableActor::ATriggerableActor(const FObjectInitializer &PCIP)
    : Super(PCIP)
{
    Collider = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collider"));
    Collider->InitSphereRadius(64.0f);
    Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    RootComponent = Collider;

    TriggerSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("TriggerSound"));
    TriggerSound->bAutoActivate = false;
    TriggerSound->AttachTo(RootComponent);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    IsActivated = false;
    CanDeactivate = true;

}

// Called when the game starts or when spawned
void ATriggerableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATriggerableActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATriggerableActor::Trigger()
{
    // Don't trigger if we cannot deactivate when already activated!
    if( !CanDeactivate && IsActivated )
        return;

    IsActivated = !IsActivated;

    TriggerSound->Play();

    // Eventually rotate lever
    if( Lever )
    {
        FVector LeverDir = Lever->GetActorRotation().Vector();
        LeverDir.Z = -LeverDir.Z;
        Lever->SetActorRotation(LeverDir.Rotation());
    }

    // Eventually rescale button
    if( Button )
    {
        float ScaleFactor = 0.5f;

        if( IsActivated )
            ScaleFactor = 2.0f;

        FVector Scale = Button->GetActorScale3D();
        Scale.X *= ScaleFactor;
        Button->SetActorScale3D(Scale);
    }

    // Broadcast through our delegate to all event listeners, that we have triggered!
    OnTriggered.Broadcast(IsActivated);
}

