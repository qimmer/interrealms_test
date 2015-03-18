// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "PlayerCharacter.h"
#include "AIPlayerControlState.h"

APlayerCharacter::APlayerCharacter()
{
    PlayerControlState = ConstructObject<UAIPlayerControlState>(UAIPlayerControlState::StaticClass(), this);
    PlayerControlState->NextState = PlayerControlState;
}

void APlayerCharacter::BeginPlay()
{
    PlayerControlState = ConstructObject<UAIPlayerControlState>(UAIPlayerControlState::StaticClass(), this);
    PlayerControlState->NextState = PlayerControlState;

    States.Reset();
    States.Add(PlayerControlState);

    PlayerControlState->Camera = CameraActor;

    Super::BeginPlay();

}

void APlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
