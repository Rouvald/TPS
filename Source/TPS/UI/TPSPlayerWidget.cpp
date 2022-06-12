// TPS Testing Project. All Rights Reserved.

#include "UI/TPSPlayerWidget.h"
#include "TPSCharacter.h"

float UTPSPlayerWidget::GetHealthPercent() const
{
    const auto Character = Cast<ATPSCharacter>(GetOwningPlayerPawn());
    return Character ? Character->GetHealthPercent() : -1.0f;
}

ESlateVisibility UTPSPlayerWidget::IsPlayerAlive() const
{
    const auto Character = Cast<ATPSCharacter>(GetOwningPlayerPawn());
    return (Character && !Character->IsDead()) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
}