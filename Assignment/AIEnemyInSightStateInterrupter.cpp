#include "Assignment.h"
#include "AIEnemyInSightStateInterrupter.h"
#include "AICharacter.h"
#include "AIAttackState.h"
#include "AIController.h"
#include "AIWaitState.h"

UAIEnemyInSightStateInterrupter::UAIEnemyInSightStateInterrupter()
{
}

void UAIEnemyInSightStateInterrupter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if( Character && StateOverride )
    {
        // If we are waiting, don't interrupt (Prevents the enemy in being to hard!)
        if( Cast<UAIWaitState>(Character->CurrentState) )
            return;

        UAIAttackState *AttackState = Cast<UAIAttackState>(StateOverride);
        if( AttackState )
        {
            // Find an eventual enemy
            AActor *Enemy = FindEnemy(Character->EnemyDetectionRange);

            if( Enemy )
            {
                // Set the attack state victim to our found enemy,
                // and interrupt the current AI state with the
                // attack state! Let's attack that son-of-a-bitch!
                AttackState->Victim = Enemy;
                Interrupt();
            }
        }
    }
}

AActor *UAIEnemyInSightStateInterrupter::FindEnemy(float Range) const
{
    if( Character )
    {
        AAIController *AIController = Cast<AAIController>(Character->GetController());
        if( AIController )
        {
            for(int32 i = 0; i < Character->Enemies.Num(); ++i )
            {
                AActor *Enemy = Character->Enemies[i];

                // If its a character, don't attack a dead one
                AAICharacter *EnemyCharacter = Cast<AAICharacter>(Enemy);
                if( EnemyCharacter && EnemyCharacter->Health <= 0.0f )
                    continue;

                // Test if the enemy is out of range
                if( (Enemy->GetActorLocation() - Character->GetActorLocation()).Size() > Range )
                    continue;

                // Test if we cannot see this enemy
                const FVector& Loc = Character->GetActorLocation();
                FVector TraceEnd = Enemy->GetActorLocation();
                FHitResult HitInfo;
                FCollisionQueryParams QParams;
                FCollisionObjectQueryParams OParams = FCollisionObjectQueryParams::AllStaticObjects;

                 if( Character->GetWorld()->LineTraceSingle(HitInfo, Loc, TraceEnd, QParams, OParams) )
                    continue;

                 // Test if the AI controller cannot navigate to the enemy
                 if( AIController->MoveToActor(Enemy) == EPathFollowingRequestResult::Failed )
                     continue;

                 // No obstacles, we found a potential enemy to attack!
                 return Enemy;
            }

        }
    }

    return nullptr;
}
