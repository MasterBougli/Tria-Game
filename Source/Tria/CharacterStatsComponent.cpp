#include "CharacterStatsComponent.h"
#include "GameFramework/Actor.h"

UCharacterStatsComponent::UCharacterStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    ResetStats();
}

void UCharacterStatsComponent::BeginPlay()
{
    Super::BeginPlay();
    CalculateFinalStats();
}

void UCharacterStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UpdateModifiers(DeltaTime);
}

float UCharacterStatsComponent::GetStat(FName StatName) const
{
    if (StatName == "Health") return BaseStats.Health;
    if (StatName == "MaxHealth") return BaseStats.MaxHealth;
    if (StatName == "Stamina") return BaseStats.Stamina;
    if (StatName == "MaxStamina") return BaseStats.MaxStamina;
    if (StatName == "Mana") return BaseStats.Mana;
    if (StatName == "MaxMana") return BaseStats.MaxMana;
    if (StatName == "Attack") return BaseStats.Attack;
    if (StatName == "Defense") return BaseStats.Defense;
    if (StatName == "Magic") return BaseStats.Magic;
    if (StatName == "MagicDefense") return BaseStats.MagicDefense;
    if (StatName == "Speed") return BaseStats.Speed;
    return 0.0f;
}

void UCharacterStatsComponent::ModifyStat(FName StatName, float Value)
{
    if (StatName == "Health")
    {
        BaseStats.Health = FMath::Clamp(BaseStats.Health + Value, 0.0f, BaseStats.MaxHealth);
        OnStatChanged.Broadcast(StatName, BaseStats.Health);
    }
    else if (StatName == "MaxHealth")
    {
        BaseStats.MaxHealth = FMath::Max(0.0f, BaseStats.MaxHealth + Value);
        BaseStats.Health = FMath::Min(BaseStats.Health, BaseStats.MaxHealth);
        OnStatChanged.Broadcast(StatName, BaseStats.MaxHealth);
    }
    // ... autres stats similaires
}

void UCharacterStatsComponent::AddStatModifier(const FStatModifier& Modifier)
{
    ActiveModifiers.Add(Modifier);
    ModifyStat(Modifier.StatName, Modifier.Value);
}

void UCharacterStatsComponent::RemoveStatModifier(const FStatModifier& Modifier)
{
    ActiveModifiers.Remove(Modifier);
    ModifyStat(Modifier.StatName, -Modifier.Value);
}

void UCharacterStatsComponent::AddExperience(float Amount)
{
    BaseStats.Experience += Amount;
    CheckLevelUp();
}

void UCharacterStatsComponent::LevelUp()
{
    BaseStats.Level++;
    
    // Augmentation des stats de base
    BaseStats.MaxHealth += 10.0f;
    BaseStats.MaxStamina += 5.0f;
    BaseStats.MaxMana += 5.0f;
    BaseStats.Attack += 2.0f;
    BaseStats.Defense += 1.0f;
    BaseStats.Magic += 2.0f;
    BaseStats.MagicDefense += 1.0f;

    // Restaurer les stats à leur maximum
    BaseStats.Health = BaseStats.MaxHealth;
    BaseStats.Stamina = BaseStats.MaxStamina;
    BaseStats.Mana = BaseStats.MaxMana;

    // Augmenter l'expérience nécessaire pour le prochain niveau
    BaseStats.ExperienceToNextLevel *= 1.5f;

    OnLevelUp.Broadcast(BaseStats.Level);
}

void UCharacterStatsComponent::UpdateModifiers(float DeltaTime)
{
    for (int32 i = ActiveModifiers.Num() - 1; i >= 0; --i)
    {
        if (!ActiveModifiers[i].IsPermanent)
        {
            ActiveModifiers[i].Duration -= DeltaTime;
            if (ActiveModifiers[i].Duration <= 0.0f)
            {
                RemoveStatModifier(ActiveModifiers[i]);
            }
        }
    }
}

void UCharacterStatsComponent::CalculateFinalStats()
{
    // Appliquer tous les modificateurs permanents
    for (const FStatModifier& Modifier : ActiveModifiers)
    {
        if (Modifier.IsPermanent)
        {
            ModifyStat(Modifier.StatName, Modifier.Value);
        }
    }
}

void UCharacterStatsComponent::CheckLevelUp()
{
    while (BaseStats.Experience >= BaseStats.ExperienceToNextLevel)
    {
        BaseStats.Experience -= BaseStats.ExperienceToNextLevel;
        LevelUp();
    }
}

void UCharacterStatsComponent::TakeDamage(float DamageAmount)
{
    float ActualDamage = FMath::Max(0.f, DamageAmount - BaseStats.Defense);
    BaseStats.Health -= ActualDamage;
    if (BaseStats.Health <= 0.f)
    {
        Die();
    }
}

float UCharacterStatsComponent::DealDamage()
{
    return BaseStats.Attack * 2.f + FMath::RandRange(0.f, BaseStats.Attack * 0.5f);
}

void UCharacterStatsComponent::ConsumeStamina(float Amount)
{
    BaseStats.Stamina = FMath::Max(0.f, BaseStats.Stamina - Amount);
}

void UCharacterStatsComponent::RegenerateStamina(float Amount)
{
    BaseStats.Stamina = FMath::Min(BaseStats.MaxStamina, BaseStats.Stamina + Amount);
}

void UCharacterStatsComponent::ConsumeMana(float Amount)
{
    BaseStats.Mana = FMath::Max(0.f, BaseStats.Mana - Amount);
}

void UCharacterStatsComponent::RegenerateMana(float Amount)
{
    BaseStats.Mana = FMath::Min(BaseStats.MaxMana, BaseStats.Mana + Amount);
}

float UCharacterStatsComponent::GetAttackSpeed() const
{
    return 1.f + BaseStats.Speed * 0.05f;
}

float UCharacterStatsComponent::GetMaxHealth() const
{
    return BaseStats.MaxHealth;
}

float UCharacterStatsComponent::GetMaxMana() const
{
    return BaseStats.MaxMana;
}

float UCharacterStatsComponent::GetSpellDamage() const
{
    return BaseStats.Magic * 2.f + BaseStats.Attack * 0.5f;
}

void UCharacterStatsComponent::Die()
{
    if (AActor* Owner = GetOwner())
    {
        Owner->Destroy();
    }
}

void UCharacterStatsComponent::ResetStats()
{
    BaseStats.Level = 1;
    BaseStats.Health = 100.0f;
    BaseStats.MaxHealth = 100.0f;
    BaseStats.Stamina = 100.0f;
    BaseStats.MaxStamina = 100.0f;
    BaseStats.Mana = 100.0f;
    BaseStats.MaxMana = 100.0f;
    BaseStats.Attack = 10.0f;
    BaseStats.Defense = 5.0f;
    BaseStats.Magic = 10.0f;
    BaseStats.MagicDefense = 5.0f;
    BaseStats.Speed = 100.0f;
    BaseStats.Experience = 0.0f;
    BaseStats.ExperienceToNextLevel = 100.0f;
    CalculateFinalStats();
} 