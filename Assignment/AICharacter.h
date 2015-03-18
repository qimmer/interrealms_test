// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class ATriggerableActor;

UCLASS()
/**
 * @brief The AAICharacter class is a base class for all game characters.
 * It provides basic AI functionality such as walking around, idling, fleeing and attacking.
 * Player characters can also inherit from this one to provide extended functionality to
 * the player character. Eventually, input controlling can be implemented as an "AI" state.
 */
class ASSIGNMENT_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    UFUNCTION()
    /**
     * @brief SetAIState Sets the current AI behaviour state
     * @param State The state to set.
     */
    void SetAIState(class UAIState *State);

    UFUNCTION()
    /**
     * @brief Pickup Tries to pick up an actor as an inventory item,
     * such as a weapon etc.
     * @param Item The actor of the item, etc. an AWeapon ...
     * This is also typically the actor which is the ItemActor
     * in an APickup.
     * @return True if the item has been picked up. False if we cannot pick
     * it up for some reason, full inventory, inpickupable actor etc.
     */
    bool Pickup(const AActor *ItemActor);

    /**
     * @brief Jump Jumps or double jumps
     */
    virtual void Jump();

    UFUNCTION()
    /**
     * @brief Attack Performs an attack / fires a shot with the current weapon,
     * or triggers a trigger if nearby.
     * @return
     */
    void Attack();

    // AI Parameters
public:
    UPROPERTY(EditAnywhere, Category = "AI")
    TArray<class AActor *> Enemies;

    UPROPERTY(EditAnywhere, Category = "AI")
    float RunSpeed;

    UPROPERTY(EditAnywhere, Category = "AI")
    float WalkSpeed;

    UPROPERTY(EditAnywhere, Category = "AI")
    float EnemyDetectionRange;
	
    UPROPERTY(EditAnywhere, Category = "AI")
    bool IsRunning;

    // Control Parameters
public:
    UPROPERTY(EditAnywhere, Category = "Control")
    /**
     * @brief DoubleJumpForce The force to add when double jumping
     */
    float DoubleJumpForce;

    UPROPERTY(EditAnywhere)
    /**
     * @brief TriggerRange The max. range this character can trigger triggers.
     */
    float TriggerRange;

    UPROPERTY(EditAnywhere, Category = "AI")
    TArray<class UAIStateInterrupter *> StateInterrupters;

    UPROPERTY(EditAnywhere, Category = "AI")
    TArray<class UAIState *> States;

protected:
    virtual void OnLanded(const FHitResult& hit);

private:
    UPROPERTY(VisibleAnywhere, Category = "AI")
    class UAIState *CurrentState;

   /**
    * @brief CanDoubleJump Is true when the player has jumped, but not double jumped yet.
    * Is always false when on ground.
    */
   UPROPERTY()
   bool CanDoubleJump;

   /**
    * @brief IsAttacking Is true while the player is attacking.
    */
   UPROPERTY()
   bool IsAttacking;

   /**
    * @brief FindTrigger
    * @return An eventual found trigger actor nearby.
    */
   ATriggerableActor * FindTrigger() const;
};
