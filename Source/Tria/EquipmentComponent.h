#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentSlot.h"
#include "ItemData.h"
#include "EquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentSlot, Slot, FName, NewItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentFailed, FName, ItemID, FString, Reason);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TRIA_API UEquipmentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEquipmentComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Équipement")
    TMap<EEquipmentSlot, FEquipmentSlotData> EquipmentSlots;

    UPROPERTY(BlueprintAssignable, Category="Équipement")
    FOnEquipmentChanged OnEquipmentChanged;

    UPROPERTY(BlueprintAssignable, Category="Équipement")
    FOnEquipmentFailed OnEquipmentFailed;

    UFUNCTION(BlueprintCallable, Category="Équipement")
    bool EquipItem(FName ItemID, EEquipmentSlot Slot);

    UFUNCTION(BlueprintCallable, Category="Équipement")
    bool UnequipItem(EEquipmentSlot Slot);

    UFUNCTION(BlueprintCallable, Category="Équipement")
    FName GetEquippedItemID(EEquipmentSlot Slot) const;

    UFUNCTION(BlueprintCallable, Category="Équipement")
    bool IsSlotCompatible(FName ItemID, EEquipmentSlot Slot) const;

    UFUNCTION(BlueprintCallable, Category="Équipement")
    bool CanEquipItem(FName ItemID, EEquipmentSlot Slot) const;

    UFUNCTION(BlueprintCallable, Category="Équipement")
    void LockSlot(EEquipmentSlot Slot, bool bLock);

    UFUNCTION(BlueprintCallable, Category="Équipement")
    FItemData GetItemData(FName ItemID) const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    UDataTable* ItemDataTable;

    void InitializeSlots();
}; 