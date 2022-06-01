// TPS Testing Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPSPlayerHUD.generated.h"

/**
 *
 */
UCLASS()
class TPS_API ATPSPlayerHUD : public AHUD
{
    GENERATED_BODY()
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerWidgetClass;

    virtual void BeginPlay() override;
};
