// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TriggerableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FTriggerableActorOnTriggered, bool, IsActive );

UCLASS()
class ASSIGNMENT_API ATriggerableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerableActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void Trigger();

    UPROPERTY(BlueprintAssignable, Category="Triggering")
    FTriggerableActorOnTriggered OnTriggered;

    UPROPERTY(EditAnywhere)
    /**
     * @brief CanDeactivate Set to true, if this trigger should be possible
     * to deactivate after activation.
     */
    bool CanDeactivate;

    UPROPERTY(EditAnywhere)
    /**
     * @brief Lever An eventual lever actor. If this actor is set,
     * the lever will be rotated -45 degrees when deactivated
     * and +45 degrees when activated.
     */
    AActor * Lever;

    UPROPERTY(EditAnywhere)
    /**
     * @brief Button An eventual button actor. If this actor is set,
     * the button will move itself 25% of it's depth backwards when activated.
     */
    AActor * Button;

    UPROPERTY(BlueprintReadOnly)
    /**
     * @brief IsActivated Is true if this trigger has been activated.
     */
    bool IsActivated;
};
