#pragma once

#include "AIState.h"
#include "AIWalkToState.generated.h"

UCLASS()
class ASSIGNMENT_API UAIWalkToState : public UAIState
{
    GENERATED_BODY()

public:
    UAIWalkToState();

    virtual void OnStateActivated();
    virtual void OnStateDeactivated();
    virtual void Tick( float DeltaSeconds );

    UPROPERTY(EditAnywhere)
    /**
     * @brief Destination The destination to walk to
     */
    FVector Destination;
};
