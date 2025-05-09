#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterCustomizationComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TRIA_API UCharacterCustomizationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCharacterCustomizationComponent();

    // Map : Partie du corps -> SkeletalMesh actuellement équipé
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Customisation")
    TMap<FName, USkeletalMesh*> CurrentMeshes;

    // Map : Partie du corps -> SkeletalMesh de base (sauvegarde)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Customisation")
    TMap<FName, USkeletalMesh*> BaseMeshes;

    // Map : Partie du corps -> SkeletalMesh précédemment équipé (pour déséquipement)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Customisation")
    TMap<FName, USkeletalMesh*> PreviousMeshes;

    UFUNCTION(BlueprintCallable, Category="Customisation")
    void SaveBaseCustomization();

    UFUNCTION(BlueprintCallable, Category="Customisation")
    void EquipArmor(const TMap<FName, USkeletalMesh*>& ArmorMeshes);

    UFUNCTION(BlueprintCallable, Category="Customisation")
    void UnequipArmor(const TMap<FName, USkeletalMesh*>& ArmorMeshes);

    UFUNCTION(BlueprintCallable, Category="Customisation")
    void RestoreBaseCustomization();

protected:
    virtual void BeginPlay() override;
}; 