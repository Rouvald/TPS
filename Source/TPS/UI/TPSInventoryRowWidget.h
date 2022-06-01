// TPS Testing Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSCoreTypes.h"
#include "TPSInventoryRowWidget.generated.h"

class UTextBlock;
/**
 *
 */
UCLASS()
class TPS_API UTPSInventoryRowWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetItemType(const EInventoryItemType& NewItemType);

    UFUNCTION(BlueprintCallable)
    FText GetItemAmount() const;

protected:
    virtual void NativeOnInitialized() override;

private:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemTypeTextBlock;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemAmountTextBlock;

    EInventoryItemType ItemType;
};
