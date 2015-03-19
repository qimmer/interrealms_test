#include "Assignment.h"
#include "AIAttackState.h"
#include "AICharacter.h"
#include "AIController.h"

UAIAttackState::UAIAttackState()
{
    AttackRange = 150.0f;
}

void UAIAttackState::OnStateActivated()
{
    Super::OnStateActivated();

    if( Character )
        Character->IsRunning = true;
}

void UAIAttackState::OnStateDeactivated()
{
    Super::OnStateDeactivated();

    if( Character )
        Character->IsRunning = false;
}

void UAIAttackState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if( Character && Victim )
    {
        if( Character->Health <= 0.0f )
        {
            Finish();
            return;
        }

        // If its a character, don't attack a dead one
        AAICharacter *VictimCharacter = Cast<AAICharacter>(Victim);
        if( VictimCharacter && VictimCharacter->Health <= 0.0f )
        {
            Finish();
            return;
        }

        AAIController *AIController = Cast<AAIController>(Character->GetController());
        if( AIController )
        {
            // We cannot attack in mid air
            if( !Character->GetMovementComponent()->IsMovingOnGround() )
            {
                AIController->StopMovement();
                Finish();
                return;
            }

            float Distance = (Character->GetActorLocation() - Victim->GetActorLocation()).Size();

            if( AIController->GetMoveStatus() != EPathFollowingStatus::Moving && Distance > AttackRange )
            {
                AIController->MoveToActor(Victim, AttackRange);
                Finish();
                return;
            }

            // Is it time to attack?
            if( Distance <= AttackRange )
            {
                AIController->StopMovement();

                if( Character->Attack() )
                {
                    Finish();
                    return;
                }
            }

            // If the enemy has escaped, finish this state!
            if( Distance > Character->EnemyDetectionRange )
            {
                AIController->StopMovement();

                Finish();
                return;
            }
        }
    }

}
