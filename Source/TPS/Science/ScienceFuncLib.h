// TPS Testing Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScienceFuncLib.generated.h"

/**
 *
 */
UCLASS()
class TPS_API UScienceFuncLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, Category = "Science")
    static int32 Fibonacci(const int32 Value);
};
