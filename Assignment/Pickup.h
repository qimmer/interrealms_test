#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class ASSIGNMENT_API APickup : public AActor
{
    GENERATED_BODY()

public:
    APickup(const FObjectInitializer &PCIP);

    virtual void BeginPlay() override;
    virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    /**
     * @brief Drop Drops the item actor and turns into a standard
     * physics-enabled actor and destroys this pickup.
     */
    void Drop();

protected:
    UFUNCTION()
    void OnTrigger(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public: // Properties
    UPROPERTY(EditAnywhere)
    AActor * ItemActor;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class USphereComponent *Trigger;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent *Container;
};
