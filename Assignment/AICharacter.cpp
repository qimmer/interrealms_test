// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "AICharacter.h"
#include "AIWaitState.h"
#include "AIWalkToState.h"
#include "CharacterAnimInstance.h"
#include "AIController.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CurrentState = nullptr;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

    WalkToState = ConstructObject<UAIWalkToState>(UAIWalkToState::StaticClass(), this);
    WaitState = ConstructObject<UAIWaitState>(UAIWaitState::StaticClass(), this);

    WalkToState->NextState = WaitState;
    WaitState->NextState = WalkToState;

    RunSpeed = 300.0f;
    WalkSpeed = 100.0f;
    IsRunning = false;
	
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    SetState(WalkToState);
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    GetCharacterMovement()->MaxWalkSpeed = IsRunning ? RunSpeed : WalkSpeed;

    if( CurrentState )
        CurrentState->Tick(DeltaTime);

    // Handle animation values
    UCharacterAnimInstance *AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->AnimScriptInstance);
    if( AnimInstance )
    {
        FVector Velocity = GetVelocity();

        // Make sure we don't consider falling as walking!
        Velocity.Z = 0.0f;

        AnimInstance->Speed = Velocity.Size() / RunSpeed;
    }

    AAIController *AIController = Cast<AAIController>(GetController());
    if( AIController )
    {
        AIController->UpdateControlRotation(DeltaTime, false);
    }

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AAICharacter::SetState(UAIState *State)
{
    if( CurrentState != nullptr )
    {
        CurrentState->OnStateDeactivated();
    }

    CurrentState = State;
    CurrentState->Character = this;
    CurrentState->OnStateActivated();
}

