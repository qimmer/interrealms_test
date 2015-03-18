// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "GameCamera.generated.h"

UCLASS()
class ASSIGNMENT_API AGameCamera : public ACameraActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
    AGameCamera(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(EditAnywhere)
    /**
     * @brief Target The target actor to focus on.
     */
    AActor * Target;

    UPROPERTY(EditAnywhere)
    /**
     * @brief TargetDistance The desired distance from the target to the camera
     */
    float TargetDistance;

    UPROPERTY(EditAnywhere)
    /**
     * @brief HorizontalModifier The amount of units to un-center the player in the view.
     * Useful to make the player able to see what's in front of the target, if the target
     * is standing in the way.
     */
    float HorizontalModifier;

    UPROPERTY(EditAnywhere)
    /**
     * @brief TargetDeltaYaw The yaw to add to the target camera rotation.
     * 0.0 means the camera will look at the target from behind.
     * 180.0 means the camera will look in front of the target.
     * This can be used in the target is something else than the player.
     */
    float DeltaYaw;

    UPROPERTY(EditAnywhere)
    /**
     * @brief TargetDeltaPitch The pitch of the camera
     */
    float DeltaPitch;

    UPROPERTY(EditAnywhere)
    /**
     * @brief TargetHeight The height of the camera
     */
    float TargetHeight;
};
