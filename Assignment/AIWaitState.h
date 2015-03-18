#pragma once

#include "AIState.h"
#include "AIWaitState.generated.h"

UCLASS()
class ASSIGNMENT_API UAIWaitState : public UAIState
{
    GENERATED_BODY()

public:
    UAIWaitState();

    virtual void OnStateActivated();
    virtual void OnStateDeactivated();
    virtual void Tick( float DeltaSeconds );

    UPROPERTY(EditAnywhere)
    /**
     * @brief Seconds The amount of seconds to wait before walking to a random waypoint.
     */
    float Seconds;

private:
    UPROPERTY()
    float Timer;
};
