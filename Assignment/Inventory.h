#pragma once

#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

USTRUCT()
struct ASSIGNMENT_API FInventory
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    int32 Capacity;

    UPROPERTY(EditAnywhere)
    TArray<AActor*> Items;
};
