// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "FloatingPlatform.generated.h"

class ATriggerableActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASSIGNMENT_API UFloatingPlatform : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFloatingPlatform();

	// Called when the game starts
	virtual void InitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    UPROPERTY(EditAnywhere)
    /**
     * @brief Offset The offset in seconds for this platform's movement
     */
    float Offset;

    UPROPERTY(EditAnywhere)
    /**
     * @brief Speed The speed this platform floats up and down
     */
    float Speed;

    UPROPERTY(EditAnywhere)
    /**
     * @brief Range The range the platform should move up and down within
     */
    float Range;

    UPROPERTY(EditAnywhere)
    /**
     * @brief ActivationTrigger An eventual trigger which controls this platform
     */
    ATriggerableActor *ActivationTrigger;
	
private:
    /**
     * @brief InitialHeight The initial Z-coordinate of this platform
     */
    float InitialHeight;
    float CurrentTime;
};
