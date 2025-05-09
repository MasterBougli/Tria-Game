#pragma once

#include "CoreMinimal.h"
#include "EquipmentSlot.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
    Head        UMETA(DisplayName = "Tête"),
    Shoulders   UMETA(DisplayName = "Épaules"),
    Chest       UMETA(DisplayName = "Torse"),
    Arms        UMETA(DisplayName = "Bras"),
    Hands       UMETA(DisplayName = "Mains"),
    Legs        UMETA(DisplayName = "Jambes"),
    Feet        UMETA(DisplayName = "Pieds"),
    MainHand    UMETA(DisplayName = "Main principale"),
    OffHand     UMETA(DisplayName = "Main secondaire"),
    Accessory1  UMETA(DisplayName = "Accessoire 1"),
    Accessory2  UMETA(DisplayName = "Accessoire 2")
};

USTRUCT(BlueprintType)
struct FEquipmentSlotData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipmentSlot Slot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName EquippedItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsLocked;
}; 