#include "Assignment.h"
#include "AIState.h"
#include "AICharacter.h"

UAIState::UAIState()
{
    NextState = nullptr;
    Character = nullptr;
}

void UAIState::OnStateActivated()
{
}

void UAIState::OnStateDeactivated()
{

}

void UAIState::Tick(float DeltaSeconds)
{
}

void UAIState::Finish()
{
    if( Character )
        Character->SetAIState(NextState);
}
