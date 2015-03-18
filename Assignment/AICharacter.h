// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class AWayPoint;
class UAIWalkToState;
class UAIWaitState;

UCLASS()
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
     * @brief setState Sets a new state
     * @param State The newly created state to set.
     */
    void SetState(class UAIState *State);

    UPROPERTY(EditAnywhere)
    float RunSpeed;

    UPROPERTY(EditAnywhere)
    float WalkSpeed;
	
    UPROPERTY(EditAnywhere)
    bool IsRunning;

private:
   UPROPERTY()
   UAIState *CurrentState;

   UPROPERTY()
   UAIWaitState *WaitState;

   UPROPERTY()
   UAIWalkToState *WalkToState;
};
