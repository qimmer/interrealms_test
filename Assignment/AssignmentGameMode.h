// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphSchema.h"
#include "AssignmentGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API AAssignmentGameMode : public AGameMode
{
	GENERATED_BODY()

	AAssignmentGameMode(const FObjectInitializer& ObjectInitializer);
	
	/** Handle stuff on game start */
	virtual void StartPlay() override;	
	
};
