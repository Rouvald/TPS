// TPS Testing Project. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSCharacterTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Engine/World.h"
#include "TPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSCharacterTests, All, All);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthMightBeChangedWithDamage, "TPSGame.Character.HealthMightBeChangedWithDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeKilled, "TPSGame.Character.CharacterCanBeKilled",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoHealShouldRestoreHealth, "TPSGame.Character.AutoHealShouldRestoreHealth",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

/* Latent Command tests*/
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleWait, "TPSGame.LatentCommand.SimpleWait",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleLog, "TPSGame.LatentCommand.SimpleLog",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandOpenCloseMap, "TPSGame.LatentCommand.OpenCloseMap",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

//

using namespace TPS::Test;

/* Test map ref: World'/Game/Tests/DefaultTestLevel.DefaultTestLevel' */
namespace
{
constexpr char* CharacterBPTestName = "Blueprint'/Game/Tests/BP_Test_ThirdPersonCharacter.BP_Test_ThirdPersonCharacter'";
}  // namespace

bool FHealthMightBeChangedWithDamage::RunTest(const FString& Parameters)
{
    LevelScope("/Game/Tests/DefaultTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull(TEXT("World exists"), World)) return false;

    const FTransform InitTransform{FVector(0.0f, -240.0f, 60.0f)};
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterBPTestName, InitTransform);
    if (!TestNotNull(TEXT("Character exists"), Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

    Character->FinishSpawning(InitTransform);

    constexpr float DamageAmount = 10.0f;
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health is decreased", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCharacterDestroyLatentCommand, ATPSCharacter*, Character, float, LifeSpan);

bool FCharacterDestroyLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= LifeSpan)
    {
        if (IsValid(Character))
        {
            UE_LOG(LogTPSCharacterTests, Error, TEXT("Character doesn't destoyed"));
        }
        return true;
    }
    return false;
}

bool FCharacterCanBeKilled::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/DefaultTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull(TEXT("World exists"), World)) return false;

    const FTransform InitTransform{FVector(0.0f, -240.0f, 60.0f)};
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterBPTestName, InitTransform);
    if (!TestNotNull(TEXT("Character exists"), Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    HealthData.LifeSpan = 2.0f;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

    Character->FinishSpawning(InitTransform);

    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);

    const auto KillingDamageAmount = HealthData.MaxHealth;
    Character->TakeDamage(KillingDamageAmount, FDamageEvent{}, nullptr, nullptr);

    TestEqual("Health is empty", Character->GetHealthPercent(), 0.0f);
    TestTrueExpr(Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None);
    ENUM_LOOP_START(ECollisionChannel, EnumElem)
    if (EnumElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(Character->GetCapsuleComponent()->GetCollisionResponseToChannel(EnumElem) == ECollisionResponse::ECR_Ignore);
    }
    ENUM_LOOP_END
    TestTrueExpr(Character->GetMesh()->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics);
    TestTrueExpr(Character->GetMesh()->IsSimulatingPhysics());
    TestTrueExpr(FMath::IsNearlyEqual(Character->GetLifeSpan(), HealthData.LifeSpan));

    // ADD_LATENT_AUTOMATION_COMMAND(FCharacterDestroyLatentCommand(Character, HealthData.LifeSpan));

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [Character]()
        {
            if (IsValid(Character))
            {
                UE_LOG(LogTPSCharacterTests, Error, TEXT("Character doesn't destoyed"));
            }
        },
        HealthData.LifeSpan));

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FAutoHealCheckLatentCommand, ATPSCharacter*, Character, float, HealDuration);

bool FAutoHealCheckLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= HealDuration)
    {
        if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
        {
            UE_LOG(LogTPSCharacterTests, Error, TEXT("Auto heal doesn't work"));
        }
        return true;
    }
    return false;
}

bool FAutoHealShouldRestoreHealth::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/DefaultTestLevel");

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull(TEXT("World exists"), World)) return false;

    const FTransform InitTransform{FVector(0.0f, -240.0f, 60.0f)};
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterBPTestName, InitTransform);
    if (!TestNotNull(TEXT("Character exists"), Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    HealthData.HealthModifier = 10.0f;
    HealthData.AutoHealRate = 1.0f;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

    Character->FinishSpawning(InitTransform);

    constexpr float DamageAmount = 20.0f;
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health is decreased", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    const float HealthDiff = HealthData.MaxHealth * (1.0f - Character->GetHealthPercent());
    const float HealDuration = HealthData.AutoHealRate * HealthDiff / HealthData.HealthModifier;
    // ADD_LATENT_AUTOMATION_COMMAND(FAutoHealCheckLatentCommand(Character, HealDuration));

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [Character]()
        {
            if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
            {
                UE_LOG(LogTPSCharacterTests, Error, TEXT("Auto heal doesn't work"));
            }
        },
        HealDuration));

    return true;
}

/* Latent command tests */
bool FLatentCommandSimpleWait::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.0f));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FTPSLogLatentCommand, FString, LogMessage);

bool FTPSLogLatentCommand::Update()
{
    UE_LOG(LogTPSCharacterTests, Display, TEXT("%s"), *LogMessage);
    return true;
}

bool FLatentCommandSimpleLog::RunTest(const FString& Parameters)
{
    UE_LOG(LogTPSCharacterTests, Display, TEXT("Log 1"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSLogLatentCommand("Latent Log 1"));
    UE_LOG(LogTPSCharacterTests, Display, TEXT("Log 2"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSLogLatentCommand("Latent Log 2"));
    UE_LOG(LogTPSCharacterTests, Display, TEXT("Log 3"));
    return true;
}

bool FLatentCommandOpenCloseMap::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/Tests/DefaultTestLevel");
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.0f));
    return true;
}

//

#endif
