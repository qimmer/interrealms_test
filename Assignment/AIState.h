#pragma once

#include "UObject.h"
#include "AIState.generated.h"

UCLASS(Blueprintable, BlueprintType)
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

    UPROPERTY(EditAnywhere)
    class UAIState *NextState;

    UPROPERTY(EditAnywhere)
    class AAICharacter *Character;
};
