// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "AssignmentGameMode.h"
#include "PlayerCharacter.h"

AAssignmentGameMode::AAssignmentGameMode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
    DefaultPawnClass = APlayerCharacter::StaticClass();
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
