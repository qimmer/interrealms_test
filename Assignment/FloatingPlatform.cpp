// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "FloatingPlatform.h"
#include "TriggerableActor.h"

// Sets default values for this component's properties
UFloatingPlatform::UFloatingPlatform()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFloatingPlatform::InitializeComponent()
{
	Super::InitializeComponent();

    InitialHeight = GetOwner()->GetActorLocation().Z;
    CurrentTime = 0.0f;
}


// Called every frame
void UFloatingPlatform::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

    if( ActivationTrigger != nullptr )
        if( !ActivationTrigger->IsActivated )
            return;

    CurrentTime += DeltaTime;

    FVector Location = GetOwner()->GetActorLocation();
    Location.Z = InitialHeight + FMath::Sin(CurrentTime * Speed) * Range / 2.0f;
    GetOwner()->SetActorLocation(Location);
}

