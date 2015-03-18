#include "Assignment.h"
#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/MeshComponent.h"

AWeapon::AWeapon(const class FObjectInitializer& PCIP)
    : Super(PCIP)
{
    PrimaryActorTick.bCanEverTick = true;

    BoxCollider = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("WeaponCollider"));
    RootComponent = BoxCollider;

    WeaponMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("WeaponMesh"));
    WeaponMesh->AttachTo(RootComponent);
}

void AWeapon::BeginPlay()
{

}

void AWeapon::Tick(float DeltaSeconds)
{

}

void AWeapon::Fire()
{

}
