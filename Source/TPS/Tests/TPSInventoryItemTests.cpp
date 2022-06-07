// TPS Testing Project. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSInventoryItemTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Items/TPSInventoryItem.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "TestUtils.h"
#include "Components/TextRenderComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "TPSGame.Items.Inventory.CppActorCantBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentsShouldBeSetupCorrectly, "TPSGame.Items.Inventory.ComponentsShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

/* Test map ref: World'/Game/Tests/DefaultTestLevel.DefaultTestLevel' */
namespace
{
constexpr char* InventoryItemBPName = "Blueprint'/Game/Items/BP_TPSInventoryItem.BP_TPSInventoryItem'";
}

using namespace TPS::Test;

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
    const FString ExpectedWarningMessage =
        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSInventoryItem::StaticClass()->GetName());
    AddExpectedError(ExpectedWarningMessage, EAutomationExpectedErrorFlags::Exact);

    LevelScope("/Game/Tests/DefaultTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull(TEXT("World exists"), World)) return false;

    const FTransform InitTransform{FVector(200.0f, 200.0f, 0.0f)};
    const ATPSInventoryItem* InvItem = World->SpawnActor<ATPSInventoryItem>(ATPSInventoryItem::StaticClass(), InitTransform);
    if (!TestNull(TEXT("Inventory item exists"), InvItem)) return false;

    return true;
}

bool FComponentsShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/DefaultTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull(TEXT("World exists"), World)) return false;

    const FTransform InitTransform{FVector(200.0f, 200.0f, 0.0f)};
    const ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPName, InitTransform);
    if (!TestNotNull(TEXT("Inventory item exists"), InvItem)) return false;

    const auto CollisionComp = InvItem->FindComponentByClass<USphereComponent>();
    if (!TestNotNull(TEXT("Sphere component exists"), CollisionComp)) return false;

    TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() >= 30.0f);
    TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
    ENUM_LOOP_START(ECollisionChannel, EnumElem)
    if (EnumElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EnumElem) == ECollisionResponse::ECR_Overlap);
    }
    ENUM_LOOP_END
    TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
    TestTrueExpr(InvItem->GetRootComponent() == CollisionComp);

    const auto MeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull(TEXT("Static mesh component exists"), MeshComp)) return false;

    TestTrueExpr(MeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    const auto TextRendComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull(TEXT("Text render component exists"), TextRendComp)) return false;

    return true;
}

#endif
