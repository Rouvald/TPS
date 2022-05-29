// TPS Testing Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace TPS
{
class TPS_API Battery
{
public:
    Battery() = default;
    Battery(float PercentAmount);

    void Charge();
    void UnCharge();

    float GetPercent() const;
    FColor GetColor() const;
    FString PercentToString() const;

    float GetMaxPercent() const { return MaxPercent; }

    bool operator>=(const Battery& rhs) const { return this->GetPercent() >= rhs.GetPercent(); }
    bool operator==(const Battery& rhs) const { return FMath::IsNearlyEqual(this->GetPercent(), rhs.GetPercent()); }

private:
    float Percent = 1.0f;
    float MaxPercent = 1.0f;

    void SetPercent(float PercentAmount);
};
}  // namespace TPS
