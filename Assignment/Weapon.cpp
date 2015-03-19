#include "Assignment.h"
#include "Components/BoxComponent.h"
#include "Components/MeshComponent.h"
#include "Projectile.h"
#include "Weapon.h"


AWeapon::AWeapon(const class FObjectInitializer& PCIP)
    : Super(PCIP)
{
    PrimaryActorTick.bCanEverTick = true;

    BoxCollider = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("WeaponCollider"));
    RootComponent = BoxCollider;

    WeaponMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("WeaponMesh"));
    WeaponMesh->AttachTo(RootComponent);

    ShootSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("ShootSound"));
    ShootSound->bAutoActivate = false;
    ShootSound->AttachTo(RootComponent);
}

void AWeapon::BeginPlay()
{

}

void AWeapon::Tick(float DeltaSeconds)
{

}

void AWeapon::Fire(AActor *Murderer)
{
    if( Data.Type == EWeapon::EProjectile )
    {
        if (Data.ProjectileClass != NULL)
        {
            UWorld* World = GetWorld();
            if (World)
            {
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = this;
                SpawnParams.Instigator = Instigator;

                // Spawn location outside the weapon collider!
                FVector SpawnLocation = GetActorLocation();
                FVector SpawnDirection = Murderer->GetActorForwardVector();
                SpawnLocation += SpawnDirection * 20.0f;

                // Shoot a bit upwards
                SpawnDirection.Z += 0.02f;
                SpawnDirection.Normalize();

                AProjectile* Projectile = World->SpawnActor<AProjectile>(Data.ProjectileClass, SpawnLocation, SpawnDirection.Rotation(), SpawnParams);
                if (Projectile)
                {
                    Projectile->Weapon = this;
                    Projectile->Fire(SpawnDirection);
                    ShootSound->Play();
                }
            }
        }
    }
}
