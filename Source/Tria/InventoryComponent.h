#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const TArray<FInventoryItem>&, NewItems);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeightChanged, float, CurrentWeight, float, MaxWeight);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TRIA_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventaire")
    TArray<FInventoryItem> Items;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventaire")
    UDataTable* ItemDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventaire")
    float MaxWeight;

    UPROPERTY(BlueprintAssignable, Category="Inventaire")
    FOnInventoryChanged OnInventoryChanged;

    UPROPERTY(BlueprintAssignable, Category="Inventaire")
    FOnWeightChanged OnWeightChanged;

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    bool AddItem(FName ItemID, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    bool RemoveItem(FName ItemID, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    bool HasItem(FName ItemID, int32 Quantity = 1) const;

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    FItemData GetItemData(FName ItemID) const;

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    void UseItem(FName ItemID);

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    void EquipItem(FName ItemID);

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    void UnequipItem(FName ItemID);

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    void UseConsumable(FName ItemID);

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    float GetCurrentWeight() const;

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    bool CanAddItem(FName ItemID, int32 Quantity = 1) const;

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    void SaveInventory();

    UFUNCTION(BlueprintCallable, Category="Inventaire")
    void LoadInventory();

protected:
    virtual void BeginPlay() override;

    void UpdateWeight();
    void NotifyInventoryChanged();
    void NotifyWeightChanged();

private:
    float CurrentWeight;
}; 