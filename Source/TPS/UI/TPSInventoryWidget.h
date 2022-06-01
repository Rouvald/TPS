// TPS Testing Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSInventoryWidget.generated.h"

class UVerticalBox;
/**
 *
 */
UCLASS()
class TPS_API UTPSInventoryWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    virtual void NativeOnInitialized() override;

private:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* ItemRowsVerBox;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> InventoryItemRowWidgetClass;

    void InitInventory() const;
};
