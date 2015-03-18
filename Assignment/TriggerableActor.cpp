// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "TriggerableActor.h"


// Sets default values
ATriggerableActor::ATriggerableActor()
{
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

