#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemData.h"
#include "WorldItem.generated.h"

UCLASS()
class TRIA_API AWorldItem : public AActor
{
    GENERATED_BODY()

public:
    AWorldItem();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* MeshComponent;

    // Liste déroulante dynamique des items de la DataTable
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(GetOptions="GetAllItemIDs"))
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UDataTable* ItemDataTable;

    UFUNCTION(BlueprintCallable, Category="Item")
    FItemData GetItemData() const;

    UFUNCTION(BlueprintCallable, Category="Item")
    void Pickup(class AActor* Picker);

#if WITH_EDITOR
    UFUNCTION(CallInEditor, Category="Item")
    TArray<FName> GetAllItemIDs() const;
#endif

protected:
    virtual void BeginPlay() override;
}; 