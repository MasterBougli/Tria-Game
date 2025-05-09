#include "CharacterCustomizationComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

UCharacterCustomizationComponent::UCharacterCustomizationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterCustomizationComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCharacterCustomizationComponent::SaveBaseCustomization()
{
    BaseMeshes = CurrentMeshes;
}

void UCharacterCustomizationComponent::EquipArmor(const TMap<FName, USkeletalMesh*>& ArmorMeshes)
{
    if (AActor* Owner = GetOwner())
    {
        for (const TPair<FName, USkeletalMesh*>& Elem : ArmorMeshes)
        {
            FName BodyPart = Elem.Key;
            USkeletalMesh* NewMesh = Elem.Value;
            if (CurrentMeshes.Contains(BodyPart))
            {
                PreviousMeshes.Add(BodyPart, CurrentMeshes[BodyPart]);
            }
            CurrentMeshes.Add(BodyPart, NewMesh);

            // Recherche du composant enfant par nom
            TArray<USkeletalMeshComponent*> Components;
            Owner->GetComponents<USkeletalMeshComponent>(Components);
            for (USkeletalMeshComponent* MeshComp : Components)
            {
                if (MeshComp && MeshComp->GetName() == BodyPart.ToString())
                {
                    MeshComp->SetSkeletalMesh(NewMesh);
                    break;
                }
            }
        }
    }
}

void UCharacterCustomizationComponent::UnequipArmor(const TMap<FName, USkeletalMesh*>& ArmorMeshes)
{
    if (AActor* Owner = GetOwner())
    {
        for (const TPair<FName, USkeletalMesh*>& Elem : ArmorMeshes)
        {
            FName BodyPart = Elem.Key;
            if (PreviousMeshes.Contains(BodyPart))
            {
                USkeletalMesh* PrevMesh = PreviousMeshes[BodyPart];
                CurrentMeshes.Add(BodyPart, PrevMesh);

                // Recherche du composant enfant par nom
                TArray<USkeletalMeshComponent*> Components;
                Owner->GetComponents<USkeletalMeshComponent>(Components);
                for (USkeletalMeshComponent* MeshComp : Components)
                {
                    if (MeshComp && MeshComp->GetName() == BodyPart.ToString())
                    {
                        MeshComp->SetSkeletalMesh(PrevMesh);
                        break;
                    }
                }
                PreviousMeshes.Remove(BodyPart);
            }
        }
    }
}

void UCharacterCustomizationComponent::RestoreBaseCustomization()
{
    if (AActor* Owner = GetOwner())
    {
        for (const TPair<FName, USkeletalMesh*>& Elem : BaseMeshes)
        {
            FName BodyPart = Elem.Key;
            USkeletalMesh* BaseMesh = Elem.Value;
            CurrentMeshes.Add(BodyPart, BaseMesh);

            // Recherche du composant enfant par nom
            TArray<USkeletalMeshComponent*> Components;
            Owner->GetComponents<USkeletalMeshComponent>(Components);
            for (USkeletalMeshComponent* MeshComp : Components)
            {
                if (MeshComp && MeshComp->GetName() == BodyPart.ToString())
                {
                    MeshComp->SetSkeletalMesh(BaseMesh);
                    break;
                }
            }
        }
        PreviousMeshes.Empty();
    }
} 