// TPS Testing Project. All Rights Reserved.

#include "Items/TPSInventoryItem.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/TPSInventoryComponent.h"

ATPSInventoryItem::ATPSInventoryItem()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    checkf(SphereComponent, TEXT("SphereComponent = nullptr"));
    SphereComponent->SetSphereRadius(30.0f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SphereComponent->SetGenerateOverlapEvents(true);
    SetRootComponent(SphereComponent);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    checkf(Mesh, TEXT("Mesh = nullptr"));
    Mesh->SetupAttachment(GetRootComponent());
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
    checkf(TextRenderComponent, TEXT("TextRenderComponent = nullptr"));
    TextRenderComponent->SetupAttachment(GetRootComponent());
    TextRenderComponent->SetRelativeLocation(FVector{0.0f, 0.0f, 40.0f});
    TextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
}

void ATPSInventoryItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DestroyItemEffect, GetActorLocation());
}

void ATPSInventoryItem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    InitInventoryItem();
}

void ATPSInventoryItem::InitInventoryItem()
{
    /* Set static mesh for current item type */
    if (ItemsMap.Contains(ItemData.InventoryItemType))
    {
        Mesh->SetStaticMesh(ItemsMap[ItemData.InventoryItemType]);
    }
    /* Update Static mesh  material */
    FLinearColor RandColor = DefaultColor;
    if (ItemColors.Num())
    {
        RandColor = ItemColors[FMath::RandRange(0, ItemColors.Num() - 1)];
    }
    if (BaseMaterialInterface)
    {
        const auto MatInstDyn = Mesh->CreateDynamicMaterialInstance(0, BaseMaterialInterface);
        if (MatInstDyn)
        {
            MatInstDyn->SetVectorParameterValue(ColorParameterName, RandColor);
        }
    }
    /* Set item amount and color in text render */
    TextRenderComponent->SetText(FText::FromString(FString::FromInt(ItemData.ItemAmount)));
    TextRenderComponent->SetTextRenderColor(RandColor.ToFColor(true));
}

void ATPSInventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (const auto Pawn = Cast<APawn>(OtherActor))
    {
        if (const auto InventoryComponent = Pawn->FindComponentByClass<UTPSInventoryComponent>())
        {
            if (InventoryComponent->TryToAddItem(ItemData))
            {
                Destroy();
            }
        }
    }
}
