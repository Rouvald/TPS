// TPS Testing Project. All Rights Reserved.

#include "Tests/SandboxTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, TEXT("TPSGame.Math.MaxInt"),
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, TEXT("TPSGame.Math.Sqrt"),
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter);

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("Max [Int] testing function");

    TestTrue("2 different positive numbers", FMath::Max<int32>(13, 25) == 25);
    TestEqual("2 equal positive numbers", FMath::Max<int32>(25, 25), 25);
    TestTrue("2 different positive numbers", FMath::Max<int32>(0, 123) == 123);
    TestEqual("2 equal positive numbers", FMath::Max<int32>(0, 0), 0);
    TestTrue("negative number and zero", FMath::Max<int32>(-2345, 0) == 0);
    TestTrue("2 different negative numbers", FMath::Max<int32>(-45, -67) == -45);
    TestEqual("2 equal negative numbers", FMath::Max<int32>(-9, -9), -9);
    // TestTrue("one positive and one negative numbers", FMath::Max<int32>(45, -19) == 45);
    TestTrueExpr(FMath::Max<int32>(45, -19) == 45);

    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    AddInfo("Sqrt testing function");
    // sqrt(3) = 1.73205
    TestEqual(TEXT("[0] Sqrt(4)"), FMath::Sqrt(4.0f), 2.0f);
    TestEqual(TEXT("[1] Sqrt(3)"), FMath::Sqrt(3.0f), 1.7f, 0.1f);
    TestEqual(TEXT("[2] Sqrt(3)"), FMath::Sqrt(3.0f), 1.73f, 0.01f);
    TestEqual(TEXT("[3] Sqrt(3)"), FMath::Sqrt(3.0f), 1.732051f, 1.e-5f);

    return true;
}
