// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Inventory.h"
#include "Weapon.h"
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
    AAICharacter(const FObjectInitializer &PCIP);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    virtual float TakeDamage(float Damage, const FDamageEvent &DamageEvent, AController *EventInstigator, AActor *DamageCauser);

    UFUNCTION()
    /**
     * @brief SetAIState Sets the current AI behaviour state
     * @param State The state to set.
     */
    void SetAIState(class UAIState *State);

    UFUNCTION()
    void SetCurrentItem(int32 InventoryIndex);

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
    bool Pickup(AActor *ItemActor);

    /**
     * @brief Drop Drops current item if any
     */
    void Drop();

    /**
     * @brief Jump Jumps or double jumps
     */
    virtual void Jump();

    UFUNCTION()
    /**
     * @brief Attack Performs an attack / fires a shot with the current weapon,
     * or triggers a trigger if nearby.
     * @return True if an attack has been performed.
     */
    bool Attack();

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
    float PunchPushPower;

    UPROPERTY(EditAnywhere, Category = "AI")
    float Health;
	
    UPROPERTY(EditAnywhere, Category = "AI")
    bool IsRunning;

    UPROPERTY(EditAnywhere, Category = "AI")
    FInventory Inventory;

    UPROPERTY(EditAnywhere, Category = "AI")
    AActor * CurrentItem;

    UPROPERTY(EditAnywhere, Category = "AI")
    FWeaponData Fist;

    // Sound
public:
    UPROPERTY(EditAnywhere, Category = "Sound")
    class UAudioComponent *StepSound;

    UPROPERTY(EditAnywhere, Category = "Sound")
    class UAudioComponent *JumpSound;

    UPROPERTY(EditAnywhere, Category = "Sound")
    class UAudioComponent *LandSound;

    UPROPERTY(EditAnywhere, Category = "Sound")
    class UAudioComponent *DamageReceiveSound;

    UPROPERTY(EditAnywhere, Category = "Sound")
    class UAudioComponent *DieSound;

    UPROPERTY(EditAnywhere, Category = "Sound")
    class UAudioComponent *HitSound;

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

    UPROPERTY(EditAnywhere)
    /**
     * @brief StepDistance The distance between step sounds
     */
    float StepDistance;

    UPROPERTY(EditAnywhere, Category = "AI")
    TArray<class UAIStateInterrupter *> StateInterrupters;

    UPROPERTY(EditAnywhere, Category = "AI")
    TArray<class UAIState *> States;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    class UAIState *CurrentState;

protected:
    virtual void OnLanded(const FHitResult& hit);

private:

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

   /**
    * @brief NextAttackTime The time left before we can attack again
    */
   float NextAttackTime;

   /**
    * @brief NextStepDelta How long the character must travel before next step sound
    */
   float NextStepDelta;
};
