// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Explosion.h"
#include "Weapon.h"

// Sets default values
AProjectile::AProjectile(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collider"));
    CollisionComp->InitSphereRadius(15.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    RootComponent = CollisionComp;

    ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComponent"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 6000.f;
    ProjectileMovement->MaxSpeed = 6000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness  = 0.3f;

    ProjectileMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ProjectileMesh"));
    ProjectileMesh->AttachTo(CollisionComp);

    InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectile::Fire(const FVector &Direction)
{
    if( ProjectileMesh && Weapon )
        ProjectileMesh->StaticMesh = Weapon->Data.ProjectileMesh;

    if (ProjectileMovement && Weapon )
    {
        if( Weapon->Data.Type == EWeapon::EProjectile )
            ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
        else
            ProjectileMovement->Velocity = Direction * 300.0f;
    }
}

void AProjectile::OnHit(AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    if ( OtherActor && (OtherActor != this) && OtherComp )
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();
        SpawnParams.Instigator = Instigator;

        AExplosion* Explosion = GetWorld()->SpawnActor<AExplosion>(AExplosion::StaticClass(), Hit.ImpactPoint, FRotator(), SpawnParams);
        if (Explosion && Weapon)
        {
            Explosion->ExplosionSound = Weapon->Data.ExplosionSound;
            Explosion->ParticleSystem->Template = Weapon->Data.ExplosionSystem;
            Explosion->Force->Radius = Weapon->Data.Damage;
        }

        Destroy();
    }
}
