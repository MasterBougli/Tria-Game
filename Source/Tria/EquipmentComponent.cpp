#include "EquipmentComponent.h"
#include "Engine/DataTable.h"
#include "CharacterCustomizationComponent.h"
#include "CharacterStatsComponent.h"

UEquipmentComponent::UEquipmentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    ItemDataTable = nullptr;
}

void UEquipmentComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializeSlots();
}

void UEquipmentComponent::InitializeSlots()
{
    EquipmentSlots.Empty();
    
    // Initialiser tous les slots d'équipement
    for (uint8 i = 0; i < static_cast<uint8>(EEquipmentSlot::Accessory2) + 1; ++i)
    {
        EEquipmentSlot Slot = static_cast<EEquipmentSlot>(i);
        FEquipmentSlotData SlotData;
        SlotData.Slot = Slot;
        SlotData.EquippedItemID = NAME_None;
        SlotData.IsLocked = false;
        EquipmentSlots.Add(Slot, SlotData);
    }
}

bool UEquipmentComponent::EquipItem(FName ItemID, EEquipmentSlot Slot)
{
    if (!CanEquipItem(ItemID, Slot))
    {
        FItemData ItemData = GetItemData(ItemID);
        FString Reason;

        if (!EquipmentSlots.Contains(Slot))
        {
            Reason = TEXT("Slot invalide");
        }
        else if (EquipmentSlots[Slot].IsLocked)
        {
            Reason = TEXT("Slot verrouillé");
        }
        else if (ItemData.ID == NAME_None)
        {
            Reason = TEXT("Item invalide");
        }
        else if (!IsSlotCompatible(ItemID, Slot))
        {
            Reason = TEXT("Slot incompatible");
        }
        else if (ItemData.RequiredLevel > 0)
        {
            if (AActor* Owner = GetOwner())
            {
                UCharacterStatsComponent* StatsComp = Owner->FindComponentByClass<UCharacterStatsComponent>();
                if (StatsComp && StatsComp->BaseStats.Level < ItemData.RequiredLevel)
                {
                    Reason = FString::Printf(TEXT("Niveau %d requis"), ItemData.RequiredLevel);
                }
            }
        }

        OnEquipmentFailed.Broadcast(ItemID, Reason);
        return false;
    }

    // Déséquiper l'item actuel si nécessaire
    if (EquipmentSlots[Slot].EquippedItemID != NAME_None)
    {
        UnequipItem(Slot);
    }

    // Équiper le nouvel item
    EquipmentSlots[Slot].EquippedItemID = ItemID;

    // Mettre à jour le mesh du personnage
    if (AActor* Owner = GetOwner())
    {
        UCharacterCustomizationComponent* CustomComp = Owner->FindComponentByClass<UCharacterCustomizationComponent>();
        if (CustomComp)
        {
            FItemData ItemData = GetItemData(ItemID);
            CustomComp->EquipArmor(ItemData.ArmorMeshes);
        }
    }

    OnEquipmentChanged.Broadcast(Slot, ItemID);
    return true;
}

bool UEquipmentComponent::UnequipItem(EEquipmentSlot Slot)
{
    if (EquipmentSlots[Slot].EquippedItemID == NAME_None)
    {
        return false;
    }

    FName OldItemID = EquipmentSlots[Slot].EquippedItemID;
    EquipmentSlots[Slot].EquippedItemID = NAME_None;

    // Mettre à jour le mesh du personnage
    if (AActor* Owner = GetOwner())
    {
        UCharacterCustomizationComponent* CustomComp = Owner->FindComponentByClass<UCharacterCustomizationComponent>();
        if (CustomComp)
        {
            FItemData ItemData = GetItemData(OldItemID);
            CustomComp->UnequipArmor(ItemData.ArmorMeshes);
        }
    }

    OnEquipmentChanged.Broadcast(Slot, NAME_None);
    return true;
}

FName UEquipmentComponent::GetEquippedItemID(EEquipmentSlot Slot) const
{
    if (const FEquipmentSlotData* SlotData = EquipmentSlots.Find(Slot))
    {
        return SlotData->EquippedItemID;
    }
    return NAME_None;
}

bool UEquipmentComponent::IsSlotCompatible(FName ItemID, EEquipmentSlot Slot) const
{
    FItemData ItemData = GetItemData(ItemID);
    return ItemData.CompatibleSlots.Contains(Slot);
}

bool UEquipmentComponent::CanEquipItem(FName ItemID, EEquipmentSlot Slot) const
{
    if (!EquipmentSlots.Contains(Slot) || EquipmentSlots[Slot].IsLocked)
    {
        return false;
    }

    FItemData ItemData = GetItemData(ItemID);
    if (ItemData.ID == NAME_None)
    {
        return false;
    }

    // Vérifier la compatibilité du slot
    if (!IsSlotCompatible(ItemID, Slot))
    {
        return false;
    }

    // Vérifier le niveau requis
    if (AActor* Owner = GetOwner())
    {
        UCharacterStatsComponent* StatsComp = Owner->FindComponentByClass<UCharacterStatsComponent>();
        if (StatsComp && ItemData.RequiredLevel > 0)
        {
            if (StatsComp->BaseStats.Level < ItemData.RequiredLevel)
            {
                return false;
            }
        }
    }

    return true;
}

void UEquipmentComponent::LockSlot(EEquipmentSlot Slot, bool bLock)
{
    if (EquipmentSlots.Contains(Slot))
    {
        EquipmentSlots[Slot].IsLocked = bLock;
    }
}

FItemData UEquipmentComponent::GetItemData(FName ItemID) const
{
    if (ItemDataTable)
    {
        static const FString Context = TEXT("EquipmentComponent");
        FItemData* Row = ItemDataTable->FindRow<FItemData>(ItemID, Context, true);
        if (Row)
        {
            return *Row;
        }
    }
    return FItemData();
} 