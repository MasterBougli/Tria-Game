#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EquipmentSlot.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Consumable UMETA(DisplayName = "Consommable"),
    Weapon     UMETA(DisplayName = "Arme"),
    Armor      UMETA(DisplayName = "Armure"),
    Material   UMETA(DisplayName = "Matériau"),
    Quest      UMETA(DisplayName = "Quête"),
    Misc       UMETA(DisplayName = "Divers")
};

UCLASS()
class TRIA_API UItemDataHelper : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(CallInEditor, Category="Item")
    static TArray<FName> GetAllItemIDs(const UObject* Context)
    {
        TArray<FName> IDs;
        if (const UDataTable* DataTable = Cast<UDataTable>(Context->GetOuter()))
        {
            IDs = DataTable->GetRowNames();
        }
        return IDs;
    }
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxStack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value;

    // Pour les armes/armures : stats spécifiques
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Attack;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Defence;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Magic;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Weight;

    // Pour les armures : map des parties du corps à modifier
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, USkeletalMesh*> ArmorMeshes;

    // Pour les consommables : liste des IDs des effets associés
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(GetOptions="GetAllEffectIDs"))
    TArray<FName> ConsumableEffectIDs;

    // Pour les équipements : slots compatibles
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EEquipmentSlot> CompatibleSlots;

    // Pour les équipements : niveau requis
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredLevel;

    // Pour les équipements : bonus de set
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SetID;

    // Pour les équipements : bonus de set
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SetBonusCount;

    // Pour les consommables : durée de l'effet
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    // Liste des composants/matériaux de l'objet
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> Components;
}; 