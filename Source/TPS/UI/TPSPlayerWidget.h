// TPS Testing Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSPlayerWidget.generated.h"

class UProgressBar;
/**
 *
 */
UCLASS()
class TPS_API UTPSPlayerWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable)
    ESlateVisibility IsPlayerAlive() const;
};
