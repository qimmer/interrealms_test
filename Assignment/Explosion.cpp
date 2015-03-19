// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Explosion.h"

// Sets default values
AExplosion::AExplosion(const FObjectInitializer &PCIP)
    : Super(PCIP)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Countdown = 0.1f;

    Force = PCIP.CreateDefaultSubobject<URadialForceComponent>(this, TEXT("RadialForce"));
    RootComponent = Force;

    ParticleSystem = PCIP.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Particles"));
    ParticleSystem->AttachTo(RootComponent);

    InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    Countdown -= DeltaTime;

    if( Countdown <= 0.0f && Force && ParticleSystem )
    {
        // Explode!

        Force->ImpulseStrength = 100.0f * Force->Radius;
        Force->ForceStrength = 5.0f * Force->Radius;
        Force->FireImpulse();
        ParticleSystem->Activate(true);


        UGameplayStatics::ApplyRadialDamageWithFalloff(
                    this,
                    Force->Radius,
                    0.0f,
                    GetActorLocation(),
                    Force->Radius * 0.1f,
                    Force->Radius,
                    1.5f,
                    TSubclassOf<UDamageType>(),
                    TArray<AActor*>(),
                    this);

        Countdown = FLT_MAX;
    }
}

