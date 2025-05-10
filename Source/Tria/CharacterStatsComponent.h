#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, FName, StatName, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUp, int32, NewLevel);

USTRUCT(BlueprintType)
struct FBaseStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Stamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxStamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Mana = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxMana = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Attack = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Defense = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Magic = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MagicDefense = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Speed = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Experience = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float ExperienceToNextLevel = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float HealthRegenRate = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float StaminaRegenRate = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float ManaRegenRate = 2.0f;
};

USTRUCT(BlueprintType)
struct FStatModifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName StatName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IsPermanent;

    bool operator==(const FStatModifier& Other) const
    {
        return StatName == Other.StatName
            && Value == Other.Value
            && Duration == Other.Duration
            && IsPermanent == Other.IsPermanent;
    }
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TRIA_API UCharacterStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCharacterStatsComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    FBaseStats BaseStats;

    UPROPERTY(BlueprintAssignable, Category="Stats")
    FOnStatChanged OnStatChanged;

    UPROPERTY(BlueprintAssignable, Category="Stats")
    FOnLevelUp OnLevelUp;

    UFUNCTION(BlueprintCallable, Category="Stats")
    float GetStat(FName StatName) const;

    UFUNCTION(BlueprintCallable, Category="Stats")
    void ModifyStat(FName StatName, float Value);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void AddStatModifier(const FStatModifier& Modifier);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void RemoveStatModifier(const FStatModifier& Modifier);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void AddExperience(float Amount);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void LevelUp();

    UFUNCTION(BlueprintCallable, Category="Stats")
    void ResetStats();

    UFUNCTION(BlueprintCallable, Category="Stats")
    void Die();

    UFUNCTION(BlueprintCallable, Category="Stats")
    float DealDamage();

    UFUNCTION(BlueprintCallable, Category="Stats")
    void TakeDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void ConsumeStamina(float Amount);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void RegenerateStamina(float Amount);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void ConsumeMana(float Amount);

    UFUNCTION(BlueprintCallable, Category="Stats")
    void RegenerateMana(float Amount);

    UFUNCTION(BlueprintCallable, Category="Stats")
    float GetAttackSpeed() const;

    UFUNCTION(BlueprintCallable, Category="Stats")
    float GetMaxHealth() const;

    UFUNCTION(BlueprintCallable, Category="Stats")
    float GetMaxMana() const;

    UFUNCTION(BlueprintCallable, Category="Stats")
    float GetSpellDamage() const;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY()
    TArray<FStatModifier> ActiveModifiers;

    void UpdateModifiers(float DeltaTime);
    void CalculateFinalStats();
    void CheckLevelUp();
}; 