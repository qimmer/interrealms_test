#include "Assignment.h"
#include "AIPlayerControlState.h"
#include "AICharacter.h"
#include "GameCamera.h"

UAIPlayerControlState::UAIPlayerControlState()
{
    Sensitivity = 1.5f;
}

void UAIPlayerControlState::OnStateActivated()
{
    Super::OnStateActivated();

    if( Character && Character->InputComponent )
    {
        Character->InputComponent->BindAxis("VerticalMove", this, &UAIPlayerControlState::OnVerticalMove);
        Character->InputComponent->BindAxis("HorizontalMove", this, &UAIPlayerControlState::OnHorizontalMove);

        Character->InputComponent->BindAction("Jump", IE_Pressed, this, &UAIPlayerControlState::OnStartJump);
        Character->InputComponent->BindAction("Attack", IE_Pressed, this, &UAIPlayerControlState::OnStartAttack);
        Character->InputComponent->BindAction("Attack", IE_Released, this, &UAIPlayerControlState::OnStopAttack);
        Character->InputComponent->BindAction("ChangeItem", IE_Pressed, this, &UAIPlayerControlState::OnChangeItem);
        Character->InputComponent->BindAction("DropItem", IE_Pressed, this, &UAIPlayerControlState::OnDropItem);

        Character->InputComponent->BindAxis("VerticalLook", this, &UAIPlayerControlState::OnVerticalLook);
        Character->InputComponent->BindAxis("HorizontalLook", this, &UAIPlayerControlState::OnHorizontalLook);
    }
}

void UAIPlayerControlState::OnStateDeactivated()
{

}

void UAIPlayerControlState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if( Character )
    {
        /** Process character movement from gamepad stick */
        if ( Character->Controller != nullptr )
        {
            FRotator Rotation;

            if( Camera )
                Rotation = Camera->GetActorRotation();

            if( MoveStickDirection.Size() > 0.01f )
            {
                FRotator DestinationRotation = Rotation;
                FVector destVector(FMath::Max(MoveStickDirection.Y, 0.0f), MoveStickDirection.X, 0.0f);
                FRotator RotationAddition = destVector.Rotation();

                DestinationRotation.Yaw += MoveStickDirection.X * 100.0f;

                Rotation = FMath::RInterpTo(Character->Controller->GetControlRotation(), DestinationRotation, DeltaSeconds, 3.0f);
                Character->Controller->SetControlRotation(Rotation);
            }

            /** Move the character */
            const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);

            if( MoveStickDirection.Y < 0.0f )
                Character->AddMovementInput(Direction, -MoveStickDirection.Size() * DeltaSeconds * Character->RunSpeed * 0.03f); // Move backwards
            else
                Character->AddMovementInput(Direction, MoveStickDirection.Size() * DeltaSeconds * Character->RunSpeed * 0.1f); // Move forwards
        }

        if( IsAttacking )
            Character->Attack();
    }

}

void UAIPlayerControlState::OnVerticalMove(float Val)
{
    MoveStickDirection.Y = Val;
}

void UAIPlayerControlState::OnHorizontalMove(float Val)
{
    if( Val < 0.0f )
        MoveStickDirection.X = -FMath::Pow(FMath::Abs(Val), Sensitivity);
    else
        MoveStickDirection.X = FMath::Pow(FMath::Abs(Val), Sensitivity);
}

void UAIPlayerControlState::OnChangeItem()
{
    if( Character )
    {
        int32 InventoryIndex;
        if( !Character->Inventory.Items.Find(Character->CurrentItem, InventoryIndex) )
            InventoryIndex = 0;

        Character->SetCurrentItem((InventoryIndex + 1) % Character->Inventory.Items.Num());
    }
}

void UAIPlayerControlState::OnDropItem()
{
    if( Character )
    {
        Character->Drop();
    }
}


void UAIPlayerControlState::OnStartJump()
{
    if( Character )
    {
        Character->Jump();
    }
}

void UAIPlayerControlState::OnStopAttack()
{
    IsAttacking = false;
}

void UAIPlayerControlState::OnStartAttack()
{
    IsAttacking = true;
}


void UAIPlayerControlState::OnVerticalLook(float Val)
{
    if( Camera )
        Camera->LookStickDirection.Y = Val;
}

void UAIPlayerControlState::OnHorizontalLook(float Val)
{
    if( Camera )
        Camera->LookStickDirection.X = Val;
}
