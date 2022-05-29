// TPS Testing Project. All Rights Reserved.

#include "Items/Battery.h"
using namespace TPS;

constexpr float ChargeAmount = 0.1f;

Battery::Battery(float PercentAmount)
{
    SetPercent(PercentAmount);
}

void Battery::Charge()
{
    SetPercent(Percent + ChargeAmount);
}

void Battery::UnCharge()
{
    SetPercent(Percent - ChargeAmount);
}

float Battery::GetPercent() const
{
    return Percent;
}

FColor Battery::GetColor() const
{
    if (Percent > 0.8f) return FColor::Green;
    if (Percent > 0.3f) return FColor::Yellow;
    return FColor::Red;
}

FString Battery::PercentToString() const
{
    return FString::Printf(TEXT("%i%%"), FMath::RoundToInt(Percent * 100));
}

void Battery::SetPercent(float PercentAmount)
{
    Percent = FMath::Clamp(PercentAmount, 0.0f, MaxPercent);
}
