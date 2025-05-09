#pragma once

#include "CoreMinimal.h"
#include "E_CursorType.generated.h"

UENUM(BlueprintType)
enum class E_CursorType : uint8
{
    Normal      UMETA(DisplayName = "Normal"),
    Attack      UMETA(DisplayName = "Attaque"),
    Interact    UMETA(DisplayName = "Interaction"),
    Dialogue    UMETA(DisplayName = "Dialogue"),
    Loot        UMETA(DisplayName = "Loot"),
    Forbidden   UMETA(DisplayName = "Interdit"),
    Custom1     UMETA(DisplayName = "Custom1"),
    Custom2     UMETA(DisplayName = "Custom2")
}; 