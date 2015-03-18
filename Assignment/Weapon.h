#pragma once

#include "GameFramework/Actor.h"
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

USTRUCT()
struct ASSIGNMENT_API FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly)
    bool IsMelee;

    UPROPERTY(EditDefaultsOnly)
    float Damage;

    UPROPERTY(EditDefaultsOnly)
    float FireInterval;

    UPROPERTY(EditDefaultsOnly)
    float Accuracy;
};

UCLASS()
class ASSIGNMENT_API AWeapon : public AActor
{
    GENERATED_BODY()

public:
    AWeapon(const FObjectInitializer &PCIP);

    virtual void BeginPlay() override;
    virtual void Tick( float DeltaSeconds ) override;
    void Fire();

public: // Properties
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FWeaponData Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent *BoxCollider;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent *WeaponMesh;
};
