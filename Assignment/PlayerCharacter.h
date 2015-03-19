// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AICharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API APlayerCharacter : public AAICharacter
{
	GENERATED_BODY()
	
public:
    APlayerCharacter(const FObjectInitializer &PCIP);

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(EditAnywhere)
    class AGameCamera *CameraActor;

    UPROPERTY(VisibleAnywhere)
    class UAIPlayerControlState *PlayerControlState;
};
