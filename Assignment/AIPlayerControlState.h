#pragma once

#include "AIState.h"
#include "AIPlayerControlState.generated.h"

UCLASS()
class ASSIGNMENT_API UAIPlayerControlState : public UAIState
{
    GENERATED_BODY()

public:
    UAIPlayerControlState();

    virtual void OnStateActivated();
    virtual void OnStateDeactivated();
    virtual void Tick( float DeltaSeconds );

    UPROPERTY(EditAnywhere)
    /**
     * @brief Camera The game camera actor to use for relative movement
     */
    class AGameCamera* Camera;

    // Input handlers
public:
    UFUNCTION()
    /**
     * @brief OnVerticalLook Handles gamepad look-stick vertical state
     * @param Val
     */
    void OnVerticalLook(float Val);

    UFUNCTION()
    /**
     * @brief OnHorizontalLook Handles gamepad look-stick horizontal state
     * @param Val
     */
    void OnHorizontalLook(float Val);

    UFUNCTION()
    /**
     * @brief OnVerticalMove Handles gamepad move-stick vertical state
     * @param Val
     */
    void OnVerticalMove(float Val);

    UFUNCTION()
    /**
     * @brief OnHorizontalMove Handles gamepad move-stick horizontal state
     * @param Val
     */
    void OnHorizontalMove(float Val);

    UFUNCTION()
    /**
     * @brief OnStartJump Handles jumping and double jumping
     */
    void OnStartJump();

    UFUNCTION()
    /**
     * @brief OnStartAttack If any TriggerableActors are nearby,
     * it will trigger them. Else, the player starts attacking.
     */
    void OnStartAttack();

    UFUNCTION()
    void OnStopAttack();

private:
    /**
     * @brief MoveStickDirection The current direction of the gamepad move stick
     */
    FVector2D MoveStickDirection;

    /**
     * @brief IsAttacking True if the attack button is down
     */
    bool IsAttacking;
};
