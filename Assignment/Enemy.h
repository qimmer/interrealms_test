// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AICharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API AEnemy : public AAICharacter
{
	GENERATED_BODY()
	
public:
    AEnemy();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

private:
    UPROPERTY()
    class UAIWalkToState *WalkToState;

    UPROPERTY()
    class UAIWaitState *WaitState;

    UPROPERTY()
    class UAIAttackState *AttackState;

    UPROPERTY()
    class UAIEnemyInSightStateInterrupter *EnemyInSightInterrupter;
	
};
