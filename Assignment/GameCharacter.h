// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "GameCharacter.generated.h"

UCLASS()
class ASSIGNMENT_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** The target this character should move to. */
	UPROPERTY()
	FVector Target;

	/** The threshold from the character to the target, which specifies when the
	* character is close enough to the target and should stop moving.
	*/
	float TargetThreshold;
	
};
