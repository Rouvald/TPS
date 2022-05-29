// TPS Testing Project. All Rights Reserved.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/SandboxTests.h"
#include "Tests/TestUtils.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, TEXT("TPSGame.Math.MaxInt"),
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, TEXT("TPSGame.Math.Sqrt"),
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSin, TEXT("TPSGame.Math.Sin"),
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter);

using namespace TPS::Test;

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("Max [Int] testing function");

    // clang-format off
    const TArray<TestPayLoad<TInterval<int32>, int32>> TestData
    {
        {{13, 25},25},
        {{25, 25}, 25},
        {{0, 123}, 123},
        {{0, 0}, 0},
        {{-2345, 0}, 0},
        {{-45, -67}, -45},
        {{-9, -9}, -9},
        {{45, -19}, 45}
    };
    // clang-format on
    for (const auto Data : TestData)
    {
        TestTrueExpr(FMath::Max<int32>(Data.TestValue.Min, Data.TestValue.Max) == Data.ExpectedValue);
    }
    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    AddInfo("Sqrt testing function");

    // clang-format off
    const TArray<TestPayLoad<float, float>> TestData
    {
        {4.0f, 2.0f},
        {3.0f, 1.7f, 0.1f},
        {3.0f, 1.73f, 0.01f},
        {3.0f, 1.732051f, 1.e-5f}
    };
    // clang-format on
    for (const auto Data : TestData)
    {
        TestTrueExpr(FMath::IsNearlyEqual(FMath::Sqrt(Data.TestValue), Data.ExpectedValue, Data.Tolerance));
    }
    return true;
}

bool FMathSin::RunTest(const FString& Parameters)
{
    AddInfo("Sin testing function");

    // clang-format off
    typedef float Degrees;
    const TArray<TestPayLoad<Degrees, float>> TestData
    {
        {Degrees{0.00f}, 0.0f},
        {Degrees{30.0f}, 0.5f},
        {Degrees{45.0f}, 0.707f},
        {Degrees{60.0f}, 0.866f},
        {Degrees{90.0f}, 1.0f}
    };
    // clang-format on
    for (const auto Data : TestData)
    {
        const auto Rad{FMath::DegreesToRadians(Data.TestValue)};
        TestTrueExpr(FMath::IsNearlyEqual(FMath::Sin(Rad), Data.ExpectedValue, 0.001f));
    }
    return true;
}

#endif
