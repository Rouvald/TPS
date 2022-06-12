#pragma once

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Engine/Blueprint.h"
#include "Tests/AutomationCommon.h"

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

template <typename T>
T* CreateBlueprint(UWorld* World, const FString& BpName, const FTransform& Transform = FTransform::Identity)
{
    const UBlueprint* BPItem = LoadObject<UBlueprint>(nullptr, *BpName);
    return (World && BPItem) ? World->SpawnActor<T>(BPItem->GeneratedClass, Transform) : nullptr;
}

template <typename T>
T* CreateBlueprintDeferred(UWorld* World, const FString& BpName, const FTransform& Transform = FTransform::Identity)
{
    const UBlueprint* BPItem = LoadObject<UBlueprint>(nullptr, *BpName);
    return (World && BPItem) ? World->SpawnActorDeferred<T>(BPItem->GeneratedClass, Transform) : nullptr;
}

class LevelScope
{
public:
    LevelScope(const FString& MapName) { AutomationOpenMap(MapName); };
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

UWorld* GetTestGameWorld();

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);
}  // namespace Test
}  // namespace TPS

#endif
