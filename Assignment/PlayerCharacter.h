// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AGameCamera;
class ATriggerableActor;

UCLASS()
/**
 * @brief The APlayerCharacter class is a character which is controlled
 * by a game pad. There should typically only be one of these in a scene.
 */
class ASSIGNMENT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

    /**
     * @brief BeginPlay Called when the game starts or when spawned
     */
	virtual void BeginPlay() override;
	
    /**
     * @brief Tick Called every frame
     * @param DeltaSeconds The time in seconds since the last frame
     */
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(EditAnywhere)
    /**
     * @brief Camera The game camera to use for relative movement
     */
    AGameCamera* Camera;

    UPROPERTY(EditAnywhere)
    /**
     * @brief MaxSpeed The max speed of the character
     */
    float MaxSpeed;

    UPROPERTY(EditAnywhere)
    /**
     * @brief DoubleJumpForce The force to add when double jumping
     */
    float DoubleJumpForce;

    UPROPERTY(EditAnywhere)
    /**
     * @brief LookSensitivity The sensitivity when looking around with the gamepad
     */
    float LookSensitivity;

    UPROPERTY(EditAnywhere)
    /**
     * @brief LookDamping The damping factor, which slowly moves the camera into proper position,
     * when the camera has been rotated when looking.
     */
    float LookDamping;

    UPROPERTY(EditAnywhere)
    /**
     * @brief TriggerRange The max. range this player can trigger triggers.
     */
    float TriggerRange;

protected:
    /**
     * @brief SetupPlayerInputComponent Binds game input to this character
     * @param InputComponent
     */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

protected: // Event receivers

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

    virtual void OnLanded(const FHitResult& hit);

private:
    /**
     * @brief MoveStickDirection The current direction of the gamepad move stick
     */
    FVector2D MoveStickDirection;

    /**
     * @brief LookStickDirection The current direction of the gamepad look stick
     */
    FVector2D LookStickDirection;

    /**
     * @brief CanDoubleJump Is true when the player has jumped, but not double jumped yet.
     * Is always false when on ground.
     */
    bool CanDoubleJump;

    /**
     * @brief IsAttacking Is true while the player is attacking.
     */
    bool IsAttacking;

    /**
     * @brief FindTrigger
     * @return An eventual found trigger actor nearby.
     */
    ATriggerableActor * FindTrigger() const;
};
