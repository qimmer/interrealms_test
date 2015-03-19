// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
/**
 * @brief The AExplosion class is an explosion actor.
 * The damage it causes equals to the radius, if the receiver is in the center.
 */
class ASSIGNMENT_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    AExplosion(const FObjectInitializer &PCIP);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class URadialForceComponent *Force;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UParticleSystemComponent *ParticleSystem;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    /**
     * @brief Countdown The number of seconds from creation that this explosion
     * explodes.
     */
    float Countdown;

};
