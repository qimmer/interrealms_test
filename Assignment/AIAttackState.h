#pragma once

#include "AIState.h"
#include "GameFramework/Character.h"
#include "AIAttackState.generated.h"

UCLASS()
class ASSIGNMENT_API UAIAttackState : public UAIState
{
    GENERATED_BODY()

public:
    UAIAttackState();

    virtual void OnStateActivated();
    virtual void OnStateDeactivated();
    virtual void Tick( float DeltaSeconds );

    UPROPERTY(EditAnywhere)
    /**
     * @brief Victim The victim actor to attack
     */
    AActor* Victim;

    UPROPERTY(EditAnywhere)
    /**
     * @brief AttackRange The range in this AI should attack. When the AI comes in this range,
     * it stops running and starts attacking. Small for melee, large for ranged such as guns.
     */
    float AttackRange;

private:
    /**
     * @brief PreviousRunState The IsRunning state on the character before we started attacking.
     * This is used, so we can revert to walk state after we have been running.
     */
    bool PreviousRunState;
};
