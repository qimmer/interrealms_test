// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "AssignmentGameMode.h"
#include "AICharacter.h"

AAssignmentGameMode::AAssignmentGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    DefaultPawnClass = AAICharacter::StaticClass();
}

void AAssignmentGameMode::StartPlay()
{
    Super::StartPlay();

    StartMatch();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Kick Ass!"));
    }
}

