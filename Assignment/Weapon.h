#pragma once

#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
namespace EWeapon
{
    enum Type
    {
        EMelee          UMETA(DisplayName = "Melee"),
        EThrown         UMETA(DisplayName = "Thrown"),
        EProjectile     UMETA(DisplayName = "Projectile")
    };
}

USTRUCT(Blueprintable, BlueprintType)
struct ASSIGNMENT_API FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    TEnumAsByte<EWeapon::Type> Type;

    UPROPERTY(EditAnywhere)
    float Damage;

    UPROPERTY(EditAnywhere)
    float FireInterval;

    UPROPERTY(EditAnywhere)
    float Accuracy;

    UPROPERTY(EditAnywhere)
    float MaxDistance;

    UPROPERTY(EditAnywhere)
    UParticleSystem *ExplosionSystem;

    UPROPERTY(EditAnywhere)
    UStaticMesh *ProjectileMesh;

    UPROPERTY(EditAnywhere)
    USoundBase *ExplosionSound;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class AProjectile> ProjectileClass;
};

UCLASS()
class ASSIGNMENT_API AWeapon : public AActor
{
    GENERATED_BODY()

public:
    AWeapon(const FObjectInitializer &PCIP);

    virtual void BeginPlay() override;
    virtual void Tick( float DeltaSeconds ) override;
    void Fire(AActor *Murderer);

public: // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FWeaponData Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent *BoxCollider;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent *WeaponMesh;

    UPROPERTY(EditAnywhere, Category = "Sound")
    class UAudioComponent *ShootSound;
};
