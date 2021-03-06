#include "Assignment.h"
#include "Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/MeshComponent.h"
#include "PlayerCharacter.h"

APickup::APickup(const FObjectInitializer &PCIP)
{
    ItemActor = nullptr;

    PrimaryActorTick.bCanEverTick = true;

    Trigger = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("WeaponCollider"));
    Trigger->SetCollisionResponseToAllChannels(ECR_Overlap);
    RootComponent = Trigger;

    Container = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("WeaponMesh"));
    Container->AttachTo(RootComponent);
    Container->SetCollisionResponseToAllChannels(ECR_Ignore);

    PickupSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("PickupSound"));
    PickupSound->bAutoActivate = false;
    PickupSound->AttachTo(RootComponent);

    Trigger->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnTrigger);
}

void APickup::BeginPlay()
{

}

void APickup::Tick(float DeltaSeconds)
{
    AddActorLocalRotation(FRotator(DeltaSeconds * 33.0f, DeltaSeconds * 45.0f, DeltaSeconds * 18.0f));

    if( ItemActor )
    {
        // Make sure physics are disabled when in a pickup
        if( ItemActor->GetRootComponent() )
        {
            UShapeComponent* Comp = Cast<UShapeComponent>(ItemActor->GetRootComponent());
            if( Comp )
            {
                Comp->SetSimulatePhysics(false);
            }
        }

        // Spin the container and the item
        FRotator ItemRotation = ItemActor->GetActorRotation();
        ItemRotation.Pitch = 0.0f;
        ItemRotation.Roll = 0.0f;
        ItemRotation.Yaw -= DeltaSeconds * 30.0f;

        ItemActor->SetActorLocation(GetActorLocation());
        ItemActor->SetActorRotation(ItemRotation);
    }
}

void APickup::Drop(bool ReenablePhysics)
{
    if( ItemActor )
    {
        // Make sure physics are reenabled when dropping
        if( ItemActor->GetRootComponent() )
        {
            UShapeComponent* Comp = Cast<UShapeComponent>(ItemActor->GetRootComponent());
            if( Comp )
            {
                if( ReenablePhysics )
                    Comp->SetSimulatePhysics(true);
            }
        }

        ItemActor = nullptr;
    }

    Destroy();
}

void APickup::OnTrigger(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
    if( PlayerCharacter )
    {
        if( PlayerCharacter->Pickup(ItemActor) )
        {
            UGameplayStatics::PlaySoundAttached(PickupSound->Sound, PlayerCharacter->GetRootComponent());
            Drop(false);
        }

    }
}
