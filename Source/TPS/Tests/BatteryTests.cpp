// TPS Testing Project. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/BatteryTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Items/Battery.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatteryTest, "TPSGame.Items.Battery",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FBatteryTest::RunTest(const FString& Parameters)
{
    using namespace TPS;

    AddInfo("Battery with default construct");
    const Battery BatteryDefault;
    TestTrueExpr(FMath::IsNearlyEqual(BatteryDefault.GetPercent(), 1.0f));
    TestTrueExpr(BatteryDefault.GetColor() == FColor::Green);
    TestTrueExpr(BatteryDefault.PercentToString().Equals("100%"));

    AddInfo("Battery with custom construct");
    const auto BatteryTestFunctor = [this](float PercentAmount, const FColor& Color, const FString& PercentString)
    {
        const Battery Battery(PercentAmount);
        // TestTrueExpr(FMath::IsNearlyEqual(Battery.GetMaxPercent(), 1.f));
        TestTrueExpr(FMath::IsNearlyEqual(Battery.GetPercent(), FMath::Clamp(PercentAmount, 0.0f, Battery.GetMaxPercent())));
        TestTrueExpr(Battery.GetColor() == Color);
        TestTrueExpr(Battery.PercentToString().Equals(PercentString));
    };

    BatteryTestFunctor(1.0f, FColor::Green, "100%");
    BatteryTestFunctor(0.51f, FColor::Yellow, "51%");
    BatteryTestFunctor(0.12f, FColor::Red, "12%");
    BatteryTestFunctor(1000.0f, FColor::Green, "100%");
    BatteryTestFunctor(-1000.0f, FColor::Red, "0%");

    AddInfo("Battery charge / uncharge");
    Battery BatteryObject(0.6f);
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));
    BatteryObject.UnCharge();
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.5f));
    BatteryObject.Charge();
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));

    AddInfo("Battery charge / uncharge. Corner cases");
    for (uint8 Index = 0; Index < 50; ++Index)
    {
        BatteryObject.UnCharge();
    }
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.0f));

    for (uint8 Index = 0; Index < 50; ++Index)
    {
        BatteryObject.Charge();
    }
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), BatteryObject.GetMaxPercent()));

    AddInfo("Battery comparison");
    const Battery BatteryLow(0.3f);
    const Battery BatteryHigh(0.9f);

    TestTrueExpr(BatteryHigh >= BatteryLow);
    TestTrueExpr(FMath::Max(BatteryHigh, BatteryLow) == BatteryHigh);

    AddInfo("Battery comparison in memory");

    TestNotSame(TEXT("Not same batteries"), BatteryHigh, BatteryLow);

    const Battery BatteryHighDuplicate(0.9f);
    TestTrueExpr(BatteryHigh == BatteryHighDuplicate);
    TestNotSame(TEXT("Not same batteries"), BatteryHigh, BatteryHighDuplicate);

    const Battery& BatteryHighRef = BatteryHigh;
    TestSame(TEXT("Same batteries"), BatteryHigh, BatteryHighRef);

    return true;
}

#endif
