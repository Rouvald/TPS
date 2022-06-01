// TPS Testing Project. All Rights Reserved.

#pragma once

#include "TPSCoreTypes.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
    EIIT_Cube = 0 UMETA(DisplayName = "Cube"),
    EIIT_Sphere UMETA(DisplayName = "Sphere"),
    EIIT_Cylinder UMETA(DisplayName = "Cylinder"),
    EIIT_Cone UMETA(DisplayName = "Cone")
};

USTRUCT(BlueprintType)
struct FInventoryItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventotyItem")
    EInventoryItemType InventoryItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventotyItem", meta = (ClampMin = "0"))
    int32 ItemAmount;
};
