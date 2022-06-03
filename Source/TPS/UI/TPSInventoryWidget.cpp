// TPS Testing Project. All Rights Reserved.

#include "UI/TPSInventoryWidget.h"
#include "TPSInventoryRowWidget.h"
#include "Components/TPSInventoryComponent.h"
#include "Components/VerticalBox.h"

void UTPSInventoryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    InitInventory();
}

void UTPSInventoryWidget::InitInventory() const
{
    if (!ItemRowsVerBox || !GetOwningPlayerPawn()) return;
    ItemRowsVerBox->ClearChildren();

    const auto InventoryComponent = GetOwningPlayerPawn()->FindComponentByClass<UTPSInventoryComponent>();
    if (!InventoryComponent) return;

    const UEnum* InvEnum = StaticEnum<EInventoryItemType>();
    if (!InvEnum) return;

    for (uint8 Index = 0; Index < InvEnum->NumEnums() - 1; ++Index)
    {
        const auto ItemRowWidget = CreateWidget<UTPSInventoryRowWidget>(GetWorld(), InventoryItemRowWidgetClass);
        if (!ItemRowWidget) continue;

        const EInventoryItemType EnumElem = static_cast<EInventoryItemType>(Index);
        ItemRowWidget->SetItemType(EnumElem);

        ItemRowsVerBox->AddChild(ItemRowWidget);
    }
}
