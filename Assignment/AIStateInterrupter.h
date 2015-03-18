#pragma once

#include "UObject.h"
#include "AIStateInterrupter.generated.h"

UCLASS(Blueprintable, BlueprintType)
/**
 * @brief The UAIStateInterrupter class is a base class for behaviour
 * that can change AI state before the state itself finishes.
 *
 * This comes practical when external elements in the world
 * will make the AI change it's state, fx. when it sees the player
 * or if another AI character interacts with this AI character.
 */
class ASSIGNMENT_API UAIStateInterrupter : public UObject
{
    GENERATED_BODY()

public:
    UAIStateInterrupter();

    virtual void Tick( float DeltaSeconds );

    UFUNCTION()
    /**
     * @brief Interrupt Interrupts the current state of the character
     * with this interrupters override state.
     */
    void Interrupt();

    UPROPERTY()
    /**
     * @brief Character The character of this state interrupter
     */
    class AAICharacter *Character;

    UPROPERTY()
    /**
     * @brief StateOverride The state to set when interruption
     * happens
     */
    class UAIState *StateOverride;
};
