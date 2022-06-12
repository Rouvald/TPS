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
    EInventoryItemType InventoryItemType = EInventoryItemType::EIIT_Sphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventotyItem", meta = (ClampMin = "0"))
    int32 ItemAmount = 10;

    FString ToString() const { return FString::Printf(TEXT("(InventoryItemType=%i,ItemAmount=%i)"), InventoryItemType, ItemAmount); }
};

USTRUCT(BlueprintType)
struct FHealthData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "1.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "0.1"))
    float HealthModifier = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "0.1", Units = "s"))
    float AutoHealRate = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "0.1", Units = "s"))
    float LifeSpan = 5.0f;
};