#include "Assignment.h"
#include "AIWaitState.h"
#include "AICharacter.h"
#include "AIWalkToState.h"

UAIWaitState::UAIWaitState()
{
    Seconds = float(FMath::Rand() % 4) + 2.0f;

}

void UAIWaitState::OnStateActivated()
{
    Super::OnStateActivated();

    Timer = 0.0f;
}

void UAIWaitState::OnStateDeactivated()
{

}

void UAIWaitState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Increment timer
    Timer += DeltaSeconds;

    // If the time has elapsed, find a new random waypoint for the character
    // to navigate to.
    if( Timer >= Seconds )
    {
        Finish();
    }
}
