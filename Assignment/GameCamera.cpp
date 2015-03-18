// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "GameCamera.h"

AGameCamera::AGameCamera(const FObjectInitializer &ObjectInitializer) : ACameraActor(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGameCamera::BeginPlay()
{
	Super::BeginPlay();
}

void AGameCamera::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

    // Handle look-around
    FRotator Rotation = GetActorRotation();
    DeltaYaw += LookStickDirection.X * LookSensitivity;
    DeltaPitch += -LookStickDirection.Y * LookSensitivity;

    DeltaYaw = FMath::Clamp(DeltaYaw, -180.0f, 180.0f);
    DeltaPitch = FMath::Clamp(DeltaPitch, -80.0f, 80.0f);

    /*HorizontalModifier += MoveStickDirection.X * DeltaTime * (90.0f - HorizontalModifier);
    HorizontalModifier *= 0.99f;
    HorizontalModifier = FMath::Max(-150.0f, FMath::Min(HorizontalModifier, 150.0f));*/

    if( Target != nullptr )
    {
        FRotator TargetRotation = Target->GetActorRotation() + FRotator(DeltaPitch, DeltaYaw, 0.0f);

        // Make the camera look down when jumping or falling
        TargetRotation.Pitch -= FMath::Max(FMath::Min(FMath::Abs(Target->GetVelocity().Z) * 0.2f, 70.0f), 0.0f);

        // Smooth the rotation change
        FRotator SmoothedRotation = FMath::Lerp(GetActorRotation(), TargetRotation, 0.05f);

        // FInd the location the camera should look at
        FVector LookAtLocation = Target->GetActorLocation();
        LookAtLocation += FVector(0, 0, TargetHeight);

        // Calculate the absolute camera location
        FVector TargetCameraLocation = LookAtLocation;
        TargetCameraLocation -= SmoothedRotation.Vector() * TargetDistance;
        TargetCameraLocation += GetTransform().TransformVectorNoScale(FVector(0.0f, HorizontalModifier, 0.0f));

        // Find the farest point the camera can be without anything collides with the camera
        // or hides the character
        FCollisionQueryParams TraceParams(FName(TEXT("")), true, this);
        TraceParams.bTraceAsyncScene = true;
        TraceParams.bReturnPhysicalMaterial = false;
        TraceParams.bTraceComplex = true;

        FHitResult Hit(ForceInit);
        GetWorld()->LineTraceSingle(Hit, LookAtLocation, TargetCameraLocation, ECC_WorldStatic, TraceParams);

        // Handle interfering world geometry
        /*if( Hit.Actor.IsValid() )
            SetActorLocation(Hit.ImpactPoint);
        else*/
            SetActorLocation(TargetCameraLocation);

        SetActorRotation(SmoothedRotation);

        if( Target->GetVelocity().Size() > 0.1f )
        {
            DeltaYaw *= LookDamping;
            DeltaPitch *= LookDamping;
        }
    }


}


