// TPS Testing Project. All Rights Reserved.

#include "Science/ScienceFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogScienceFuncLib, All, All);

int32 UScienceFuncLib::Fibonacci(const int32 Value)
{
    if (Value < 0)
    {
        UE_LOG(LogScienceFuncLib, Error, TEXT("Fibonacci func: Invalid input Value = %i"), Value);
    }
    return Value <= 1 ? Value : Fibonacci(Value - 1) + Fibonacci(Value - 2);
}
