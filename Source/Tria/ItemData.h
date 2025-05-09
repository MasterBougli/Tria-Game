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

    // Pour les consommables : ID de l'effet associé
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ConsumableEffectID;

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
}; 