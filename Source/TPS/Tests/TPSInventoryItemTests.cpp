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
#include "TPSCharacter.h"
#include "Components/TPSInventoryComponent.h"
#include "Kismet/GameplayStatics.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "TPSGame.Items.Inventory.CppActorCantBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "TPSGame.Items.Inventory.BlueprintShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectly, "TPSGame.Items.Inventory.InventoryDataShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTaken, "TPSGame.Items.Inventory.InventoryItemCanBeTaken",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEveryInventoryItemMeshExists, "TPSGame.Items.Inventory.EveryInventoryItemMeshExists",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

/* Test map ref: World'/Game/Tests/DefaultTestLevel.DefaultTestLevel' */
namespace
{
constexpr char* InventoryItemBPName = "Blueprint'/Game/Items/BP_TPSInventoryItem.BP_TPSInventoryItem'";
constexpr char* InventoryItemBPTestName = "Blueprint'/Game/Tests/BP_Test_TPSInventoryItem.BP_Test_TPSInventoryItem'";
}  // namespace

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

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
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

bool FInventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/DefaultTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull(TEXT("World exists"), World)) return false;

    const FTransform InitTransform{FVector(300.0f, 300.0f, 0.0f)};
    ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitTransform);
    if (!TestNotNull(TEXT("Inventory item exists"), InvItem)) return false;

    /*const FVector Vec = {120.0f, 3330.0f, 10.0f};
    CallFuncByNameWithParams(InvItem, "TestTypesEvent",
    {
        FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), Vec.X, Vec.Y, Vec.Z) //
    });*/

    constexpr FInventoryItemData InvData{EInventoryItemType::EIIT_Cone, 300};
    const FLinearColor InvColor = FLinearColor::Yellow;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), InvColor.ToString()});
    TestTrueExpr(InvItem->GetItemInventoryData().ToString().Equals(InvData.ToString()));

    /* Test Text render component */
    const auto TextRendComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull(TEXT("Text render component exists"), TextRendComp)) return false;

    TestTrueExpr(TextRendComp->Text.ToString().Equals(FString::FromInt(InvData.ItemAmount)));
    TestTrueExpr(TextRendComp->TextRenderColor == InvColor.ToFColor(true));

    /* Test Static mesh component */
    const auto MeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull(TEXT("Static mesh component exists"), MeshComp)) return false;

    const auto Material = MeshComp->GetMaterial(0);
    if (!TestNotNull(TEXT("Material exist"), Material)) return false;

    FLinearColor MatColor;
    Material->GetVectorParameterValue(FHashedMaterialParameterInfo{"Color"}, MatColor);
    TestTrueExpr(MatColor == InvColor);

    return true;
}

bool FInventoryItemCanBeTaken::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/DefaultTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull(TEXT("World exists"), World)) return false;

    const FTransform InitTransform{FVector(300.0f, 300.0f, 0.0f)};
    ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitTransform);
    if (!TestNotNull(TEXT("Inventory item exists"), InvItem)) return false;

    constexpr FInventoryItemData InvData{EInventoryItemType::EIIT_Cone, 300};
    const FLinearColor InvColor = FLinearColor::Yellow;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), InvColor.ToString()});
    TestTrueExpr(InvItem->GetItemInventoryData().ToString().Equals(InvData.ToString()));

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSCharacter::StaticClass(), Pawns);

    const auto Character = Cast<ATPSCharacter>(Pawns[0]);
    if (!TestNotNull(TEXT("Character exists"), Character)) return false;

    const auto InvComp = Character->FindComponentByClass<UTPSInventoryComponent>();
    if (!TestNotNull(TEXT("Inventory component exists"), InvComp)) return false;
    TestTrueExpr(InvComp->GetInventoryAmountByType(InvData.InventoryItemType) == 0);

    /* Character takes inventory item */
    Character->SetActorLocation(InitTransform.GetLocation());

    TestTrueExpr(InvComp->GetInventoryAmountByType(InvData.InventoryItemType) == InvData.ItemAmount);
    TestTrueExpr(!IsValid(InvItem));

    TArray<AActor*> Items;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), Items);
    TestTrueExpr(Items.Num() == 0);

    return true;
}

bool FEveryInventoryItemMeshExists::RunTest(const FString& Parameters)
{
    AutomationOpenMap("/Game/Tests/DefaultTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull(TEXT("World exists"), World)) return false;

    ENUM_LOOP_START(EInventoryItemType, EElem)
    const FTransform InitTransform{FVector(100.0f * (Index + 1))};
    ATPSInventoryItem* InvItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBPTestName, InitTransform);
    if (!TestNotNull(TEXT("Inventory item exists"), InvItem)) return false;

    FInventoryItemData InvData{EElem, 300};
    const FLinearColor InvColor = FLinearColor::Blue;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), InvColor.ToString()});
    TestTrueExpr(InvItem->GetItemInventoryData().ToString().Equals(InvData.ToString()));

    /* Test Static mesh component */
    const auto MeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull(TEXT("Static mesh component exists"), MeshComp)) return false;

    const FString MeshMsg = FString::Printf(TEXT("Static mesh for %s exists"), *UEnum::GetValueAsString(EElem));
    TestNotNull(*MeshMsg, MeshComp->GetStaticMesh());

    ENUM_LOOP_END

    return true;
}

#endif
