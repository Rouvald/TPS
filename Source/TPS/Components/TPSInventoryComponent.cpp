// TPS Testing Project. All Rights Reserved.

#include "Components/TPSInventoryComponent.h"

UTPSInventoryComponent::UTPSInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTPSInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    const UEnum* InvEnum = StaticEnum<EInventoryItemType>();
    checkf(InvEnum, TEXT("InvEnum == nullptr"));
    for (uint8 Index = 0; Index < InvEnum->NumEnums() - 1; ++Index)
    {
        const EInventoryItemType EnumElem = static_cast<EInventoryItemType>(Index);
        const bool LimitCheckCondition = InventoryItemsLimits.Contains(EnumElem) && InventoryItemsLimits[EnumElem] > 0;
        checkf(LimitCheckCondition, TEXT("Limit for %s doesn't exist or less then 0"), *UEnum::GetValueAsString(EnumElem));
    }
}

bool UTPSInventoryComponent::TryToAddItem(const FInventoryItemData& ItemData)
{
    if (!CanAddItem(ItemData)) return false;

    if (!InventoryItemsMap.Contains(ItemData.InventoryItemType))
    {
        InventoryItemsMap.Add(ItemData.InventoryItemType, 0);
    }
    const auto NextAmount = InventoryItemsMap[ItemData.InventoryItemType] + ItemData.ItemAmount;
    if (NextAmount > InventoryItemsLimits[ItemData.InventoryItemType]) return false;

    InventoryItemsMap[ItemData.InventoryItemType] = NextAmount;
    return true;
}

bool UTPSInventoryComponent::CanAddItem(const FInventoryItemData& ItemData) const
{
    return ItemData.ItemAmount > 0;
}

int32 UTPSInventoryComponent::GetInventoryAmountByType(EInventoryItemType ItemType) const
{
    return InventoryItemsMap.Contains(ItemType) ? InventoryItemsMap[ItemType] : 0;
}
