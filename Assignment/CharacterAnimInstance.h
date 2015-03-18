// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)
class ASSIGNMENT_API UCharacterAnimInstance : public UAnimInstance
{
    GENERATED_UCLASS_BODY()

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=BlendFactors)
    float AttackFactor;
	
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=BlendFactors)
    float ExamineFactor;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=BlendFactors)
    bool IsHoldingGun;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=BlendFactors)
    float Speed;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=BlendFactors)
    uint32 IdleVariation;
};
