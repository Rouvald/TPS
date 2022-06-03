// TPS Testing Project. All Rights Reserved.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "TPS/Tests/TPSInventoryComponentTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Components/TPSInventoryComponent.h"
#include "TPSCoreTypes.h"
#include "Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCouldBeCreated, "TPSGame.Components.Inventory.ComponentCouldBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemAmountsShouldBeZeroByDefault, "TPSGame.Components.Inventory.ItemAmountsShouldBeZeroByDefault",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

/*IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCanAddItem, "TPSGame.Components.Inventory.CanAddItem",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);*/

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeAmountShouldntBeAdded, "TPSGame.Components.Inventory.NegativeAmountShouldntBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveAmountShouldBeAdded, "TPSGame.Components.Inventory.PositiveAmountShouldBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAmountMoreThanLimitCanntBeAdded, "TPSGame.Components.Inventory.AmountMoreThanLimitCanntBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{
class UTPSInventoryComponentTestable : public UTPSInventoryComponent
{
public:
    void SetInventoryItemsLimits(const TMap<EInventoryItemType, int32>& NewLimits) { InventoryItemsLimits = NewLimits; }
};

TMap<EInventoryItemType, int32> InitLimits(UTPSInventoryComponentTestable* InvComp, int32 Limit)
{
    TMap<EInventoryItemType, int32> InventoryLimits;
    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    InventoryLimits.Add(EnumElem, Limit);
    ENUM_LOOP_END
    InvComp->SetInventoryItemsLimits(InventoryLimits);
    return InventoryLimits;
}
}  // namespace

bool FComponentCouldBeCreated::RunTest(const FString& Parameters)
{
    const UTPSInventoryComponent* InventoryComponent = NewObject<UTPSInventoryComponent>();
    if (!TestNotNull(TEXT("Inventory Components can exists"), InventoryComponent)) return false;
    return true;
}

bool FItemAmountsShouldBeZeroByDefault::RunTest(const FString& Parameters)
{
    const UTPSInventoryComponent* InventoryComponent = NewObject<UTPSInventoryComponent>();
    if (!TestNotNull(TEXT("Inventory Components can exists"), InventoryComponent)) return false;

    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == 0);
    ENUM_LOOP_END

    return true;
}

/*bool FCanAddItem::RunTest(const FString& Parameters)
{
    UTPSInventoryComponentTestable* InventoryComponent = NewObject<UTPSInventoryComponentTestable>();
    if (!TestNotNull(TEXT("Inventory Components can exists"), InventoryComponent)) return false;

    const FInt32Interval AmountInterval = {-50, 50};

    AddInfo(TEXT("Negative amounts and zero testing"));
    ENUM_LOOP_START(EInventoryItemType, EnumElem)
        for (uint8 i = AmountInterval.Min; i <= 0; ++i)
        {
            TestTrueExpr(!InventoryComponent->CanAddItem({EnumElem, i}));
        }
    ENUM_LOOP_END

    AddInfo(TEXT("Positive amounts testing"));
    ENUM_LOOP_START(EInventoryItemType, EnumElem)
        for (uint8 i = 1; i <= AmountInterval.Max; ++i)
        {
            TestTrueExpr(InventoryComponent->CanAddItem({EnumElem, i}));
        }
    ENUM_LOOP_END

    return true;
}*/

bool FNegativeAmountShouldntBeAdded::RunTest(const FString& Parameters)
{
    UTPSInventoryComponentTestable* InventoryComponent = NewObject<UTPSInventoryComponentTestable>();
    if (!TestNotNull(TEXT("Inventory Components can exists"), InventoryComponent)) return false;

    InitLimits(InventoryComponent, 200);

    constexpr int32 InitialAmount = 20, NegativeAmount = -20;
    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    TestTrueExpr(InventoryComponent->TryToAddItem({EnumElem, InitialAmount}));
    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == InitialAmount);
    TestTrueExpr(!InventoryComponent->TryToAddItem({EnumElem, NegativeAmount}));
    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == InitialAmount);
    ENUM_LOOP_END

    return true;
}

bool FPositiveAmountShouldBeAdded::RunTest(const FString& Parameters)
{
    UTPSInventoryComponentTestable* InventoryComponent = NewObject<UTPSInventoryComponentTestable>();
    if (!TestNotNull(TEXT("Inventory Components can exists"), InventoryComponent)) return false;

    const auto InventoryLimits = InitLimits(InventoryComponent, 200);

    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    for (uint8 i = 0; i < InventoryLimits[EnumElem]; ++i)
    {
        TestTrueExpr(InventoryComponent->TryToAddItem({EnumElem, 1}));
        TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == i + 1);
    }
    ENUM_LOOP_END

    return true;
}

bool FAmountMoreThanLimitCanntBeAdded::RunTest(const FString& Parameters)
{
    UTPSInventoryComponentTestable* InventoryComponent = NewObject<UTPSInventoryComponentTestable>();
    if (!TestNotNull(TEXT("Inventory Components can exists"), InventoryComponent)) return false;

    constexpr int32 AmountLimit = 200;
    InitLimits(InventoryComponent, AmountLimit);

    constexpr int32 InitialAmount = 20;
    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    TestTrueExpr(InventoryComponent->TryToAddItem({EnumElem, InitialAmount}));
    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == InitialAmount);
    TestTrueExpr(!InventoryComponent->TryToAddItem({EnumElem, AmountLimit + 1}));
    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == InitialAmount);
    ENUM_LOOP_END

    return true;
}

#endif
