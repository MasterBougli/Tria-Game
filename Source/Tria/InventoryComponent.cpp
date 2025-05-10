#include "InventoryComponent.h"
#include "Engine/DataTable.h"
#include "CharacterCustomizationComponent.h"
#include "CharacterStatsComponent.h"
#include "Engine/World.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    MaxWeight = 100.0f;
    CurrentWeight = 0.0f;
    ItemDataTable = nullptr;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    UpdateWeight();
}

bool UInventoryComponent::AddItem(FName ItemID, int32 Quantity)
{
    if (!CanAddItem(ItemID, Quantity))
    {
        return false;
    }

    for (FInventoryItem& Item : Items)
    {
        if (Item.ItemID == ItemID)
        {
            Item.Quantity += Quantity;
            UpdateWeight();
            NotifyInventoryChanged();
            return true;
        }
    }
    Items.Add({ItemID, Quantity});
    UpdateWeight();
    NotifyInventoryChanged();
    return true;
}

bool UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        if (Items[i].ItemID == ItemID)
        {
            if (Items[i].Quantity > Quantity)
            {
                Items[i].Quantity -= Quantity;
                UpdateWeight();
                NotifyInventoryChanged();
                return true;
            }
            else if (Items[i].Quantity == Quantity)
            {
                Items.RemoveAt(i);
                UpdateWeight();
                NotifyInventoryChanged();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

bool UInventoryComponent::HasItem(FName ItemID, int32 Quantity) const
{
    for (const FInventoryItem& Item : Items)
    {
        if (Item.ItemID == ItemID && Item.Quantity >= Quantity)
        {
            return true;
        }
    }
    return false;
}

FItemData UInventoryComponent::GetItemData(FName ItemID) const
{
    if (ItemDataTable)
    {
        static const FString Context = TEXT("InventoryComponent");
        FItemData* Row = ItemDataTable->FindRow<FItemData>(ItemID, Context, true);
        if (Row)
        {
            return *Row;
        }
    }
    return FItemData();
}

void UInventoryComponent::UseItem(FName ItemID)
{
    if (HasItem(ItemID, 1))
    {
        FItemData Data = GetItemData(ItemID);
        if (Data.Type == EItemType::Consumable)
        {
            RemoveItem(ItemID, 1);
            UseConsumable(ItemID);
        }
    }
}

void UInventoryComponent::EquipItem(FName ItemID)
{
    FItemData Data = GetItemData(ItemID);
    if (Data.Type == EItemType::Armor)
    {
        if (AActor* Owner = GetOwner())
        {
            UCharacterCustomizationComponent* CustomComp = Owner->FindComponentByClass<UCharacterCustomizationComponent>();
            if (CustomComp)
            {
                CustomComp->EquipArmor(Data.ArmorMeshes);
            }
        }
    }
}

void UInventoryComponent::UnequipItem(FName ItemID)
{
    FItemData Data = GetItemData(ItemID);
    if (Data.Type == EItemType::Armor)
    {
        if (AActor* Owner = GetOwner())
        {
            UCharacterCustomizationComponent* CustomComp = Owner->FindComponentByClass<UCharacterCustomizationComponent>();
            if (CustomComp)
            {
                CustomComp->UnequipArmor(Data.ArmorMeshes);
            }
        }
    }
}

void UInventoryComponent::UseConsumable(FName ItemID)
{
    FItemData Data = GetItemData(ItemID);
    if (Data.Type == EItemType::Consumable)
    {
        if (AActor* Owner = GetOwner())
        {
            UCharacterStatsComponent* StatsComp = Owner->FindComponentByClass<UCharacterStatsComponent>();
            if (StatsComp)
            {
                // Appliquer tous les effets du consommable
                for (const FName& EffectID : Data.ConsumableEffectIDs)
                {
                    // Créer le modificateur de stats pour chaque effet
                    FStatModifier Modifier;
                    Modifier.StatName = EffectID;
                    Modifier.Value = Data.Value;
                    Modifier.Duration = Data.Duration;
                    Modifier.IsPermanent = false;

                    // Appliquer l'effet
                    StatsComp->AddStatModifier(Modifier);
                }
            }
        }
    }
}

float UInventoryComponent::GetCurrentWeight() const
{
    return CurrentWeight;
}

bool UInventoryComponent::CanAddItem(FName ItemID, int32 Quantity) const
{
    FItemData Data = GetItemData(ItemID);
    float NewWeight = CurrentWeight + (Data.Weight * Quantity);
    return NewWeight <= MaxWeight;
}

void UInventoryComponent::UpdateWeight()
{
    float NewWeight = 0.0f;
    for (const FInventoryItem& Item : Items)
    {
        FItemData Data = GetItemData(Item.ItemID);
        NewWeight += Data.Weight * Item.Quantity;
    }
    CurrentWeight = NewWeight;
    NotifyWeightChanged();
}

void UInventoryComponent::NotifyInventoryChanged()
{
    OnInventoryChanged.Broadcast(Items);
}

void UInventoryComponent::NotifyWeightChanged()
{
    OnWeightChanged.Broadcast(CurrentWeight, MaxWeight);
}

void UInventoryComponent::SaveInventory()
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    TArray<TSharedPtr<FJsonValue>> ItemsArray;

    for (const FInventoryItem& Item : Items)
    {
        TSharedPtr<FJsonObject> ItemObject = MakeShared<FJsonObject>();
        ItemObject->SetStringField(TEXT("ItemID"), Item.ItemID.ToString());
        ItemObject->SetNumberField(TEXT("Quantity"), Item.Quantity);
        ItemsArray.Add(MakeShared<FJsonValueObject>(ItemObject));
    }

    JsonObject->SetArrayField(TEXT("Items"), ItemsArray);
    JsonObject->SetNumberField(TEXT("MaxWeight"), MaxWeight);
    JsonObject->SetNumberField(TEXT("CurrentWeight"), CurrentWeight);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    FString SavePath = FPaths::ProjectSavedDir() / TEXT("Inventory.json");
    FFileHelper::SaveStringToFile(OutputString, *SavePath);
}

void UInventoryComponent::LoadInventory()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("Inventory.json");
    FString JsonString;
    if (FFileHelper::LoadFileToString(JsonString, *SavePath))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            Items.Empty();
            const TArray<TSharedPtr<FJsonValue>>* ItemsArray;
            if (JsonObject->TryGetArrayField(TEXT("Items"), ItemsArray))
            {
                for (const TSharedPtr<FJsonValue>& ItemValue : *ItemsArray)
                {
                    TSharedPtr<FJsonObject> ItemObject = ItemValue->AsObject();
                    FInventoryItem Item;
                    Item.ItemID = FName(*ItemObject->GetStringField(TEXT("ItemID")));
                    Item.Quantity = ItemObject->GetIntegerField(TEXT("Quantity"));
                    Items.Add(Item);
                }
            }

            MaxWeight = JsonObject->GetNumberField(TEXT("MaxWeight"));
            CurrentWeight = JsonObject->GetNumberField(TEXT("CurrentWeight"));

            NotifyInventoryChanged();
            NotifyWeightChanged();
        }
    }
} 