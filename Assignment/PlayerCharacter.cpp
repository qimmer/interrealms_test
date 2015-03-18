// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "PlayerCharacter.h"
#include "GameCamera.h"
#include "TriggerableActor.h"
#include "CharacterAnimInstance.h"

APlayerCharacter::APlayerCharacter()
{
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    /** Process character movement from gamepad stick */
    if ( Controller != nullptr )
    {
        FRotator Rotation;

        if( Camera != nullptr )
        {
            Rotation = Camera->GetActorRotation();
            Camera->DeltaYaw += LookStickDirection.X * LookSensitivity;
            Camera->DeltaPitch += -LookStickDirection.Y * LookSensitivity;

            if( MoveStickDirection.Size() > 0.01f )
            {
                Camera->DeltaYaw *= LookDamping;
                Camera->DeltaPitch *= LookDamping;
            }

            Camera->HorizontalModifier += MoveStickDirection.X * DeltaTime * (90.0f - Camera->HorizontalModifier);
            Camera->HorizontalModifier *= 0.99f;
            Camera->HorizontalModifier = FMath::Max(-150.0f, FMath::Min(Camera->HorizontalModifier, 150.0f));
        }

        if( GetCharacterMovement()->IsMovingOnGround() )
        {
            if( MoveStickDirection.Size() > 0.01f )
            {
                FRotator DestinationRotation = Rotation;
                FVector destVector(FMath::Max(MoveStickDirection.Y, -0.3f), MoveStickDirection.X, 0.0f);
                FRotator RotationAddition = destVector.Rotation();

                DestinationRotation.Yaw += RotationAddition.Yaw;

                Rotation = FMath::RInterpTo(Controller->GetControlRotation(), DestinationRotation, DeltaTime, 3.0f);
                Controller->SetControlRotation(Rotation);
            }

            /** Move the character forward */
            const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
            AddMovementInput(Direction, FMath::Max(FMath::Abs(MoveStickDirection.X), FMath::Abs(MoveStickDirection.Y)) * DeltaTime * MaxSpeed);
        }
    }

    // Handle animation values
    UCharacterAnimInstance *AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->AnimScriptInstance);
    if( AnimInstance )
    {
        AnimInstance->AttackFactor = IsAttacking ? 1.0f : 0.0f;
        AnimInstance->Speed = MoveStickDirection.Size();
    }
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    InputComponent->BindAxis("VerticalMove", this, &APlayerCharacter::OnVerticalMove);
    InputComponent->BindAxis("HorizontalMove", this, &APlayerCharacter::OnHorizontalMove);
    InputComponent->BindAxis("VerticalLook", this, &APlayerCharacter::OnVerticalLook);
    InputComponent->BindAxis("HorizontalLook", this, &APlayerCharacter::OnHorizontalLook);
    InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::OnStartJump);
    InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::OnStartAttack);
    InputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacter::OnStopAttack);
}

void APlayerCharacter::OnVerticalMove(float Val)
{
    MoveStickDirection.Y = Val;
}

void APlayerCharacter::OnHorizontalMove(float Val)
{
    MoveStickDirection.X = Val;
}

void APlayerCharacter::OnVerticalLook(float Val)
{
    LookStickDirection.Y = Val;
}

void APlayerCharacter::OnHorizontalLook(float Val)
{
    LookStickDirection.X = Val;
}

void APlayerCharacter::OnStartJump()
{
    if( CanDoubleJump )
    {
        LaunchCharacter(FVector(0, 0, DoubleJumpForce), false, false);
        CanDoubleJump = false;
    }

    if( GetCharacterMovement()->IsMovingOnGround() )
    {
        Jump();
        CanDoubleJump = true;
    }

}

void APlayerCharacter::OnStopAttack()
{
    IsAttacking = false;
}

void APlayerCharacter::OnStartAttack()
{
    // First, if we have a nearby triggerable actor, trigger it instead
    // of attacking
    ATriggerableActor *Trigger = FindTrigger();
    if( Trigger )
    {
        Trigger->Trigger();
        return;
    }

    // Now, if we reach here, there were no triggers, and we wan't to attack!
    IsAttacking = true;
}

void APlayerCharacter::OnLanded(const FHitResult &hit)
{
    CanDoubleJump = false;
}

ATriggerableActor *APlayerCharacter::FindTrigger() const
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


