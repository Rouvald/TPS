// TPS Testing Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSCoreTypes.h"
#include "TPSInventoryItem.generated.h"

class USphereComponent;
class UTextRenderComponent;
class UNiagaraSystem;

UCLASS(Abstract)
class TPS_API ATPSInventoryItem : public AActor
{
    GENERATED_BODY()

public:
    ATPSInventoryItem();

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
    USphereComponent* SphereComponent;

    UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, Category = "Component", meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* TextRenderComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
    FInventoryItemData ItemData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
    TMap<EInventoryItemType, UStaticMesh*> ItemsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
    UMaterialInterface* BaseMaterialInterface;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
    FName ColorParameterName = "Color";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
    FLinearColor DefaultColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
    TArray<FLinearColor> ItemColors;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
    UNiagaraSystem* DestroyItemEffect;

    virtual void OnConstruction(const FTransform& Transform) override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void InitInventoryItem();
};
