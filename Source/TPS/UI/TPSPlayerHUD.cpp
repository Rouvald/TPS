// TPS Testing Project. All Rights Reserved.

#include "UI/TPSPlayerHUD.h"

#include "TPSPlayerWidget.h"
#include "Blueprint/UserWidget.h"

void ATPSPlayerHUD::BeginPlay()
{
    Super::BeginPlay();

    if (const auto PlayerWidget = CreateWidget<UTPSPlayerWidget>(GetWorld(), PlayerWidgetClass))
    {
        PlayerWidget->AddToViewport();
        PlayerWidget->SetVisibility(ESlateVisibility::Visible);
    }
}
