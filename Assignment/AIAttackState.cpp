#include "Assignment.h"
#include "AIAttackState.h"
#include "AICharacter.h"
#include "AIController.h"

UAIAttackState::UAIAttackState()
{
    AttackRange = 10.0f;
}

void UAIAttackState::OnStateActivated()
{
    Super::OnStateActivated();

    if( Character )
    {
        PreviousRunState = Character->IsRunning;
        Character->IsRunning = true;
    }
}

void UAIAttackState::OnStateDeactivated()
{
    if( Character )
        Character->IsRunning = PreviousRunState;
}

void UAIAttackState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if( Character && Victim )
    {
        AAIController *AIController = Cast<AAIController>(Character->GetController());
        if( AIController )
        {
            float Distance = (Character->GetActorLocation() - Victim->GetActorLocation()).Size();

            if( AIController->GetMoveStatus() != EPathFollowingStatus::Moving && Distance > AttackRange )
            {
                AIController->MoveToActor(Victim, AttackRange);
            }

            // If the enemy has escaped, finish this state!
            if( Distance > Character->EnemyDetectionRange )
                Finish();
        }
    }

}
