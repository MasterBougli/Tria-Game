#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ConsumableEffectData.generated.h"

UENUM(BlueprintType)
enum class EConsumableEffectType : uint8
{
    Heal        UMETA(DisplayName = "Soin"),
    Stamina     UMETA(DisplayName = "Stamina"),
    Mana        UMETA(DisplayName = "Mana"),
    Buff        UMETA(DisplayName = "Buff"),
    Debuff      UMETA(DisplayName = "Debuff"),
    Custom      UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FConsumableEffectData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName EffectID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EConsumableEffectType EffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
}; 