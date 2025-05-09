#pragma once

#include "CoreMinimal.h"
#include "E_CrosshairType.generated.h"

UENUM(BlueprintType)
enum class E_CrosshairType : uint8
{
    Normal      UMETA(DisplayName = "Normal"),
    Hit         UMETA(DisplayName = "Touché"),
    Interact    UMETA(DisplayName = "Interaction"),
    Dialogue    UMETA(DisplayName = "Dialogue"),
    Loot        UMETA(DisplayName = "Loot"),
    Forbidden   UMETA(DisplayName = "Interdit"),
    Custom1     UMETA(DisplayName = "Custom1"),
    Custom2     UMETA(DisplayName = "Custom2")
}; 