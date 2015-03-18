// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "Enemy.h"
#include "AIEnemyInSightStateInterrupter.h"
#include "AIWaitState.h"
#include "AIWalkToState.h"
#include "AIAttackState.h"

AEnemy::AEnemy()
{

}

void AEnemy::BeginPlay()
{
    States.Reset();
    StateInterrupters.Reset();

    // Initialize states and their order
    WalkToState = ConstructObject<UAIWalkToState>(UAIWalkToState::StaticClass(), this);
    WaitState = ConstructObject<UAIWaitState>(UAIWaitState::StaticClass(), this);
    AttackState = ConstructObject<UAIAttackState>(UAIAttackState::StaticClass(), this);

    WaitState->NextState = WalkToState;
    WalkToState->NextState = WaitState;
    AttackState->NextState = WaitState;

    // Add the states to the state container
    States.Add(WalkToState);
    States.Add(WaitState);
    States.Add(AttackState);

    // Initialize interrupters
    EnemyInSightInterrupter = ConstructObject<UAIEnemyInSightStateInterrupter>(UAIEnemyInSightStateInterrupter::StaticClass(), this);
    EnemyInSightInterrupter->StateOverride = AttackState;

    // Add the interrupters to the container
    StateInterrupters.Add(EnemyInSightInterrupter);

    Super::BeginPlay();
}

void AEnemy::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
