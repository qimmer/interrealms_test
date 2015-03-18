#include "Assignment.h"
#include "AIWalkToState.h"
#include "AICharacter.h"
#include "AIController.h"

UAIWalkToState::UAIWalkToState()
{

}

void UAIWalkToState::OnStateActivated()
{
    Super::OnStateActivated();

    UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(Character->GetWorld());

    if( Character && NavSys )
    {
        FNavLocation Result;
        if( NavSys->GetRandomPointInRadius(Character->GetActorLocation(), 600.0f, Result) )
        {
            Destination = Result.Location;

            AAIController *AIController = Cast<AAIController>(Character->GetController());
            if( AIController )
            {
                if( AIController->GetMoveStatus() != EPathFollowingStatus::Moving )
                {
                    AIController->MoveToLocation(Destination);
                }
            }
        }
        else
        {
            // Could not find a random point to walk to.
            Finish();
        }
    }
}

void UAIWalkToState::OnStateDeactivated()
{

}

void UAIWalkToState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if( Character )
    {
        AAIController *AIController = Cast<AAIController>(Character->GetController());
        if( AIController )
        {
            if( AIController->GetMoveStatus() != EPathFollowingStatus::Moving )
            {
                Finish();
            }
        }
    }

}
