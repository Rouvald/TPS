// TPS Testing Project. All Rights Reserved.

#include "UI/TPSInventoryRowWidget.h"
#include "Components/TPSInventoryComponent.h"
#include "TPSCharacter.h"
#include "Components/TextBlock.h"

void UTPSInventoryRowWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ItemTypeTextBlock)
    {
        ItemTypeTextBlock->SetVisibility(ESlateVisibility::Visible);
    }
    if (ItemAmountTextBlock)
    {
        ItemAmountTextBlock->SetVisibility(ESlateVisibility::Visible);
    }
}

void UTPSInventoryRowWidget::SetItemType(const EInventoryItemType& NewItemType)
{
    if (ItemTypeTextBlock)
    {
        ItemType = NewItemType;
        ItemTypeTextBlock->SetText(UEnum::GetDisplayValueAsText(NewItemType));
    }
}
FText UTPSInventoryRowWidget::GetItemAmount() const
{
    const auto Player = Cast<ATPSCharacter>(GetOwningPlayerPawn());
    if (!Player) return FText::FromString("Error");

    const auto InventoryComponent = Player->FindComponentByClass<UTPSInventoryComponent>();
    if (!InventoryComponent) return FText::FromString("Error");

    return FText::FromString(FString::FromInt(InventoryComponent->GetInventoryAmountByType(ItemType)));
}