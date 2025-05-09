#include "WorldItem.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "InventoryComponent.h"

AWorldItem::AWorldItem()
{
    PrimaryActorTick.bCanEverTick = false;
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    Quantity = 1;
    ItemID = NAME_None;
    ItemDataTable = nullptr;
}

void AWorldItem::BeginPlay()
{
    Super::BeginPlay();
    // Optionnel : mettre à jour le mesh selon l'item
    if (ItemDataTable && ItemID != NAME_None)
    {
        FItemData Data = GetItemData();
        if (Data.Mesh)
        {
            MeshComponent->SetStaticMesh(Data.Mesh);
        }
    }
}

FItemData AWorldItem::GetItemData() const
{
    if (ItemDataTable)
    {
        static const FString Context = TEXT("WorldItem");
        FItemData* Row = ItemDataTable->FindRow<FItemData>(ItemID, Context, true);
        if (Row)
        {
            return *Row;
        }
    }
    return FItemData();
}

void AWorldItem::Pickup(AActor* Picker)
{
    if (!Picker) return;
    UInventoryComponent* Inventory = Picker->FindComponentByClass<UInventoryComponent>();
    if (Inventory && ItemID != NAME_None)
    {
        Inventory->AddItem(ItemID, Quantity);
        Destroy();
    }
}

#if WITH_EDITOR
TArray<FName> AWorldItem::GetAllItemIDs() const
{
    TArray<FName> IDs;
    if (ItemDataTable)
    {
        TArray<FName> RowNames = ItemDataTable->GetRowNames();
        IDs.Append(RowNames);
    }
    return IDs;
}
#endif 