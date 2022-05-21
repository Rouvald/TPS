// TPS Testing Project. All Rights Reserved.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/ScienceFuncLibTests.h"
#include "Tests/TestUtils.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Science/ScienceFuncLib.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScienceFibonacciSimple, TEXT("TPSGame.Science.Fibonacci.Simple"),
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScienceFibonacciStress, TEXT("TPSGame.Science.Fibonacci.Stress"),
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::LowPriority | EAutomationTestFlags::StressFilter);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScienceFibonacciLogHasErrors, TEXT("TPSGame.Science.Fibonacci.LogHasErrors"),
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter);

using namespace TPSGame;

bool FScienceFibonacciSimple::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci simple testing");

    // 0 1 1 2 3 5 8 13 21 34

    const TArray<TestPayLoad<int32, int32>> TestData{
        {0, 0},  //
        {1, 1},  //
        {2, 1},  //
        {3, 2},  //
        {4, 3},  //
        {5, 5},  //
        {6, 8},
    };
    for (const auto Data : TestData)
    {
        FString InfoString{FString::Printf(TEXT("TestValue = %i, ExpectedValue = %i"), Data.TestValue, Data.ExpectedValue)};
        TestEqual(InfoString, UScienceFuncLib::Fibonacci(Data.TestValue), Data.ExpectedValue);
    }
    return true;
}

bool FScienceFibonacciStress::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci stress testing");

    /*for (int32 Index = 2; Index <= 40; ++Index)
    {
        TestTrueExpr(UScienceFuncLib::Fibonacci(Index) == UScienceFuncLib::Fibonacci(Index-2) + UScienceFuncLib::Fibonacci(Index-1));
    }*/

    int32 PrevPrevValue{0}, PrevValue{1};
    for (int32 Index = 2; Index <= 40; ++Index)
    {
        const int32 CurrentValue{UScienceFuncLib::Fibonacci(Index)};
        TestTrueExpr(CurrentValue == PrevPrevValue + PrevValue);

        PrevPrevValue = PrevValue;
        PrevValue = CurrentValue;
    }
    return true;
}

bool FScienceFibonacciLogHasErrors::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci log has errors testing");

    AddExpectedError("Fibonacci func: Invalid input Value", EAutomationExpectedErrorFlags::Contains, 1);
    UScienceFuncLib::Fibonacci(-10);
    return true;
}

#endif
