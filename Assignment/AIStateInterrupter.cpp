#include "Assignment.h"
#include "AIStateInterrupter.h"
#include "AICharacter.h"

UAIStateInterrupter::UAIStateInterrupter()
{
    Character = nullptr;
}

void UAIStateInterrupter::Tick(float DeltaSeconds)
{
}

void UAIStateInterrupter::Interrupt()
{
    if( Character && StateOverride && Character->CurrentState != StateOverride )
    {
        Character->SetAIState(StateOverride);
    }
}
