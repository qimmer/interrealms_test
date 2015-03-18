#pragma once

#include "AIStateInterrupter.h"
#include "AIEnemyInSightStateInterrupter.generated.h"

UCLASS()
/**
 * @brief The UAIEnemyInSightStateInterrupter class is an interrupter,
 * which changes to an attack state, if an enemy is in sight,
 * such as fx the player.
 */
class ASSIGNMENT_API UAIEnemyInSightStateInterrupter : public UAIStateInterrupter
{
    GENERATED_BODY()

public:
    UAIEnemyInSightStateInterrupter();

    virtual void Tick( float DeltaSeconds );

    UPROPERTY(EditAnywhere)
    /**
     * @brief AttackRadius The radius where enemies have to be in in order
     * for this AI character to see them.
     */
    float AttackRadius;

private:
    /**
     * @brief FindEnemy Searches for an enemy to attack.
     * @return nullptr if no enemy is found. Else it returns
     * the found enemy
     */
    class AActor * FindEnemy(float Range) const;
};
