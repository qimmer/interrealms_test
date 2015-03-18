#pragma once

#include "UObject.h"
#include "AIState.generated.h"

UCLASS()
class ASSIGNMENT_API UAIState : public UObject
{
    GENERATED_BODY()

public:
    UAIState();

    virtual void OnStateActivated();
    virtual void OnStateDeactivated();
    virtual void Tick( float DeltaSeconds );

    UFUNCTION()
    void Finish();

    UPROPERTY()
    class UAIState *NextState;

    UPROPERTY()
    class AAICharacter *Character;
};
