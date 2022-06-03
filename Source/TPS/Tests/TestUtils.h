#pragma once

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

namespace TPS
{
namespace Test
{
template <typename Type1, typename Type2>
struct TestPayLoad
{
    Type1 TestValue;
    Type2 ExpectedValue;
    float Tolerance = KINDA_SMALL_NUMBER;
};

#define ENUM_LOOP_START(TYPE, EnumElem)                                        \
    for (uint8 Index = 0; Index < StaticEnum<TYPE>()->NumEnums() - 1; ++Index) \
    {                                                                          \
        const auto EnumElem = static_cast<TYPE>(Index);
#define ENUM_LOOP_END }
}  // namespace Test
}  // namespace TPS

#endif