// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "AICharacter.h"

#include "CharacterAnimInstance.h"
#include "AIController.h"
#include "TriggerableActor.h"
#include "Weapon.h"

#include "AIState.h"
#include "AIStateInterrupter.h"

// Sets default values
AAICharacter::AAICharacter(const FObjectInitializer &PCIP)
    : Super(PCIP)
{
    StepSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("StepSound"));
    StepSound->bAutoActivate = false;
    StepSound->AttachTo(RootComponent);

    JumpSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("JumpSound"));
    JumpSound->bAutoActivate = false;
    JumpSound->AttachTo(RootComponent);

    LandSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("LandSound"));
    LandSound->bAutoActivate = false;
    LandSound->AttachTo(RootComponent);

    DamageReceiveSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("DamageReceiveSound"));
    DamageReceiveSound->bAutoActivate = false;
    DamageReceiveSound->AttachTo(RootComponent);

    DieSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("DieSound"));
    DieSound->bAutoActivate = false;
    DieSound->AttachTo(RootComponent);

    HitSound = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("HitSound"));
    HitSound->bAutoActivate = false;
    HitSound->AttachTo(RootComponent);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Default AI parameters
    Health = 100.0f;
    RunSpeed = 300.0f;
    WalkSpeed = 100.0f;
    PunchPushPower = 20.0f;
    IsRunning = false;
    CurrentState = nullptr;
    Inventory.Capacity = 8;

    Fist.Accuracy = 1.0f;
    Fist.Damage = 0.2f;
    Fist.FireInterval = 1.0f;
    Fist.Type = EWeapon::EMelee;
    Fist.MaxDistance = 160.0f;

    StepDistance = 130.0f;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

    // Set character in all interrupters
    for(int32 i = 0; i < StateInterrupters.Num(); ++i )
        StateInterrupters[i]->Character = this;

    // Make sure the AI character rotates smmoothly
    GetCharacterMovement()->bUseControllerDesiredRotation = true;

    // Set initial state
    if( States.Num() > 0 )
        SetAIState(States[0]);
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    NextAttackTime -= DeltaTime;

    // Set max speed according to whether we are running or walking
    GetCharacterMovement()->MaxWalkSpeed = IsRunning ? RunSpeed : WalkSpeed;

    // Update the current state
    if( CurrentState )
        CurrentState->Tick(DeltaTime);

    for(int32 i = 0; i < StateInterrupters.Num(); ++i )
    {
        if(StateInterrupters[i])
            StateInterrupters[i]->Tick(DeltaTime);
    }

    // Handle animation values
    UCharacterAnimInstance *AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->AnimScriptInstance);
    if( AnimInstance )
    {

        FVector Velocity = GetVelocity();

        // Make sure we don't consider falling as walking!
        Velocity.Z = 0.0f;

        if( FVector::DotProduct(Velocity, GetActorForwardVector()) <= 0.0f )
            AnimInstance->Speed = -Velocity.Size() / RunSpeed;
        else
            AnimInstance->Speed = FMath::Min(Velocity.Size() / RunSpeed, 1.5f);

        if( CurrentItem )
            AnimInstance->IsHoldingGun = Cast<AWeapon>(CurrentItem) != nullptr;
        else
            AnimInstance->IsHoldingGun = false;

        if( AnimInstance->AttackFactor > 0.0f )
            AnimInstance->AttackFactor -= DeltaTime;

        AnimInstance->IsDead = Health <= 0.0f;

        NextStepDelta -= Velocity.Size() * DeltaTime;
    }

    // Smoothen AI controller rotation
    AAIController *AIController = Cast<AAIController>(GetController());
    if( AIController )
    {
        AIController->UpdateControlRotation(DeltaTime, false);
    }

    // Play step sound
    if( NextStepDelta <= 0.0f && GetMovementComponent()->IsMovingOnGround() )
    {
        UGameplayStatics::PlaySoundAttached(StepSound->Sound, RootComponent);
        NextStepDelta = StepDistance;
    }
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

float AAICharacter::TakeDamage(float Damage, const FDamageEvent &DamageEvent, AController *EventInstigator, AActor *DamageCauser)
{
    Health -= Damage;

    // Launch Character
    LaunchCharacter(GetActorForwardVector() * -Damage, false, false);
    GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, FString::Printf__VA(TEXT("Health: %.1f (- %.1f)"), Health, Damage));

    return Damage;
}

void AAICharacter::SetAIState(UAIState *State)
{
    // First, deactivate old state
    if( CurrentState != nullptr )
    {
        CurrentState->OnStateDeactivated();
    }

    // Initialize new state
    CurrentState = State;
    CurrentState->Character = this;
    CurrentState->OnStateActivated();
}

void AAICharacter::SetCurrentItem(int32 InventoryIndex)
{
    if( CurrentItem )
    {
        CurrentItem->SetActorEnableCollision(false);
        CurrentItem->SetActorHiddenInGame(true);
    }

    if( InventoryIndex >= 0 && InventoryIndex < Inventory.Items.Num() )
    {
        CurrentItem = Inventory.Items[InventoryIndex];

        CurrentItem->SetActorEnableCollision(false);
        CurrentItem->SetActorRelativeLocation(FVector(0, 0, 0));
        CurrentItem->SetActorRelativeRotation(FRotator(0, 0, 0));
        CurrentItem->SetActorRelativeScale3D(FVector(1, 1, 1));
        CurrentItem->AttachRootComponentTo(GetMesh(), TEXT("ItemSocket"));
        CurrentItem->SetActorHiddenInGame(false);
    }
}

bool AAICharacter::Pickup(AActor *ItemActor)
{
    if( !ItemActor )
        return false;

    // If we have got no space, don't pick up
    if( Inventory.Items.Num() >= Inventory.Capacity )
        return false;

    // Make sure physics are disabled while it is picked up
    if( ItemActor->GetRootComponent() )
    {
        UShapeComponent* Comp = Cast<UShapeComponent>(ItemActor->GetRootComponent());
        if( Comp )
        {
            Comp->SetSimulatePhysics(false);
            Comp->Deactivate();
        }
    }

    ItemActor->SetActorEnableCollision(false);
    ItemActor->SetActorHiddenInGame(true);

    SetCurrentItem(Inventory.Items.Add(ItemActor));

    return true;
}

void AAICharacter::Drop()
{
    if( CurrentItem )
    {
        int32 Index;
        if( Inventory.Items.Find(CurrentItem, Index) )
        {
            Inventory.Items.RemoveAt(Index);

            if( CurrentItem->GetRootComponent() )
            {
                UShapeComponent* Comp = Cast<UShapeComponent>(CurrentItem->GetRootComponent());
                if( Comp )
                {
                    Comp->Activate();
                    Comp->SetSimulatePhysics(true);
                }
            }

            CurrentItem->SetActorEnableCollision(true);
            CurrentItem->SetActorHiddenInGame(false);
            CurrentItem = nullptr;
        }
    }
}

void AAICharacter::Jump()
{
    if( CanDoubleJump )
    {
        LaunchCharacter(FVector(0, 0, DoubleJumpForce), false, false);
        CanDoubleJump = false;
    }

    if( GetCharacterMovement() )
    {
        if( GetCharacterMovement()->IsMovingOnGround() )
        {
            Super::Jump();
            CanDoubleJump = true;
        }
    }
}

bool AAICharacter::Attack()
{
    // If we havent waited long enough since the last attack, ignore this attack.
    if( NextAttackTime > 0.0f )
        return false;

    // First, if we have a nearby triggerable actor, trigger it instead
    // of attacking
    ATriggerableActor *Trigger = FindTrigger();
    if( Trigger )
    {
        Trigger->Trigger();
        NextAttackTime = 1.0f;
        return false;
    }

    // If we reach here, attack/shoot!

    // Shoot/throw if we are holding a weapon

    if( CurrentItem && Cast<AWeapon>(CurrentItem) != nullptr )
    {
        AWeapon *Weapon = Cast<AWeapon>(CurrentItem);
        NextAttackTime = Weapon->Data.FireInterval;
        Weapon->Fire(this);
    }
    else // Else, melee attack
    {
        NextAttackTime = Fist.FireInterval;

        HitSound->Play();

        UCharacterAnimInstance *AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->AnimScriptInstance);
        if( AnimInstance )
            AnimInstance->AttackFactor = 1.0f;

        const FVector& Loc = GetActorLocation();
        FVector TraceEnd = GetActorLocation() + GetActorForwardVector() * Fist.MaxDistance;
        FHitResult HitInfo;
        FCollisionQueryParams QParams;
        QParams.AddIgnoredActor(this);
        FCollisionObjectQueryParams OParams = FCollisionObjectQueryParams::AllObjects;

        // Find opponent
        if( GetWorld()->LineTraceSingle(HitInfo, Loc, TraceEnd, QParams, OParams) )
        {
            FVector PushVector = GetActorForwardVector() * PunchPushPower + FVector(0, 0, PunchPushPower * 1.5f);

            ACharacter *Opponent = Cast<ACharacter>(HitInfo.Actor.Get());
            if( Opponent )
            {

                Opponent->TakeDamage(Fist.Damage * PunchPushPower, FDamageEvent(), Controller, this);
            }
            else
            {
                // Push actor
                if( HitInfo.Actor.Get()->GetRootComponent() )
                {
                    UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(HitInfo.Actor.Get()->GetRootComponent());
                    if( Comp )
                    {
                        Comp->AddImpulse(PushVector * 1000.0f);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void AAICharacter::OnLanded(const FHitResult &hit)
{
    CanDoubleJump = false;
}

ATriggerableActor *AAICharacter::FindTrigger() const
{
    // First, see if we wan't to trigger a trigger.
    static FName NAME_TriggerQuery = FName(TEXT("TriggerQuery"));

    // Create a simple overlap query excluding ourself.
    FCollisionQueryParams SphereParams(NAME_TriggerQuery, false, this);

    // Array for the actors the overlap finds
    TArray<FOverlapResult> Overlaps;

    // Make the Query
    GetWorld()->OverlapMulti(Overlaps, GetActorLocation(), FQuat::Identity, FCollisionShape::MakeSphere(TriggerRange), SphereParams, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects));

    // The array will now contain a list of actors
    for (int32 Idx = 0; Idx < Overlaps.Num(); ++Idx)
    {
        FOverlapResult const& Overlap = Overlaps[Idx];
        ATriggerableActor* TriggerActor = Cast<ATriggerableActor>(Overlap.GetActor());
        // If this actor is valid, do something with it
        if( TriggerActor != nullptr )
        {
            return TriggerActor;
        }
    }

    return nullptr;
}
