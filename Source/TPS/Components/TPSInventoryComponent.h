// TPS Testing Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSCoreTypes.h"
#include "TPSInventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPS_API UTPSInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSInventoryComponent();

    bool TryToAddItem(const FInventoryItemData& ItemData);

    int32 GetInventoryAmountByType(EInventoryItemType ItemType) const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
    TMap<EInventoryItemType, int32> InventoryItemsLimits;

    virtual void BeginPlay() override;

private:
    TMap<EInventoryItemType, int32> InventoryItemsMap;

    bool CanAddItem(const FInventoryItemData& ItemData) const;
};
