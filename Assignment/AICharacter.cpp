// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "AICharacter.h"

#include "CharacterAnimInstance.h"
#include "AIController.h"
#include "TriggerableActor.h"

#include "AIState.h"
#include "AIStateInterrupter.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Default AI parameters
    RunSpeed = 300.0f;
    WalkSpeed = 100.0f;
    IsRunning = false;
    CurrentState = nullptr;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

    // Set character in all interrupters
    for(int32 i = 0; i < StateInterrupters.Num(); ++i )
        StateInterrupters[i]->Character = this;

    // Make sure the AI character rotates smmoothly
    GetCharacterMovement()->bUseControllerDesiredRotation = true;

    // Set initial state
    if( States.Num() > 0 )
        SetAIState(States[0]);
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    // Set max speed according to whether we are running or walking
    GetCharacterMovement()->MaxWalkSpeed = IsRunning ? RunSpeed : WalkSpeed;

    // Update the current state
    if( CurrentState )
        CurrentState->Tick(DeltaTime);

    for(int32 i = 0; i < StateInterrupters.Num(); ++i )
    {
        if(StateInterrupters[i])
            StateInterrupters[i]->Tick(DeltaTime);
    }

    // Handle animation values
    UCharacterAnimInstance *AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->AnimScriptInstance);
    if( AnimInstance )
    {
        AnimInstance->AttackFactor = IsAttacking ? 1.0f : 0.0f;

        FVector Velocity = GetVelocity();

        // Make sure we don't consider falling as walking!
        Velocity.Z = 0.0f;

        if( FVector::DotProduct(Velocity, GetActorForwardVector()) <= 0.0f )
            AnimInstance->Speed = -Velocity.Size() / RunSpeed;
        else
            AnimInstance->Speed = FMath::Min(Velocity.Size() / RunSpeed, 1.5f);
    }

    // Smoothen AI controller rotation
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

void AAICharacter::SetAIState(UAIState *State)
{
    // First, deactivate old state
    if( CurrentState != nullptr )
    {
        CurrentState->OnStateDeactivated();
    }

    // Initialize new state
    CurrentState = State;
    CurrentState->Character = this;
    CurrentState->OnStateActivated();
}

bool AAICharacter::Pickup(const AActor *ItemActor)
{
    return true;
}

void AAICharacter::Jump()
{
    if( CanDoubleJump )
    {
        LaunchCharacter(FVector(0, 0, DoubleJumpForce), false, false);
        CanDoubleJump = false;
    }

    if( GetCharacterMovement() )
    {
        if( GetCharacterMovement()->IsMovingOnGround() )
        {
            Super::Jump();
            CanDoubleJump = true;
        }
    }
}

void AAICharacter::Attack()
{
    // First, if we have a nearby triggerable actor, trigger it instead
    // of attacking
    ATriggerableActor *Trigger = FindTrigger();
    if( Trigger )
    {
        Trigger->Trigger();
        return;
    }

    // If we reach here, attack/shoot!
}

void AAICharacter::OnLanded(const FHitResult &hit)
{
    CanDoubleJump = false;
}

ATriggerableActor *AAICharacter::FindTrigger() const
{
    // First, see if we wan't to trigger a trigger.
    static FName NAME_TriggerQuery = FName(TEXT("TriggerQuery"));

    // Create a simple overlap query excluding ourself.
    FCollisionQueryParams SphereParams(NAME_TriggerQuery, false, this);

    // Array for the actors the overlap finds
    TArray<FOverlapResult> Overlaps;

    // Make the Query
    GetWorld()->OverlapMulti(Overlaps, GetActorLocation(), FQuat::Identity, FCollisionShape::MakeSphere(TriggerRange), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));

    // The array will now contain a list of actors
    for (int32 Idx = 0; Idx < Overlaps.Num(); ++Idx)
    {
        FOverlapResult const& Overlap = Overlaps[Idx];
        ATriggerableActor* TriggerActor = Cast<ATriggerableActor>(Overlap.GetActor());
        // If this actor is valid, do something with it
        if( TriggerActor != nullptr )
        {
            return TriggerActor;
        }
    }

    return nullptr;
}
