// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PA_AbilitySystemGenerics.generated.h"

class UGameplayEffect;
class UGameplayAbility;
/**
 * PrimaryDataAsset holding generic GAS assets used across the project.
 * Responsibilities:
 * - Provide references to common GameplayEffects (full heal, death, initial passives)
 * - Provide passive abilities granted to characters
 * - Provide base stat data table and experience progression curve
 */
UCLASS()
class UPA_AbilitySystemGenerics : public UPrimaryDataAsset
{
    GENERATED_BODY()
public:
    /** Effect that restores attributes (e.g., full health/mana) */
    FORCEINLINE TSubclassOf<UGameplayEffect> GetFullStatEffect() const { return FullStatEffect; }
    /** Effect applied on death to mark state or handle cleanup */
    FORCEINLINE TSubclassOf<UGameplayEffect> GetDeathEffect() const { return DeathEffect; }
    /** Effects applied when ASC/Avatar initializes (auras, baseline buffs) */
    FORCEINLINE const TArray<TSubclassOf<UGameplayEffect>>& GetInitialEffects() const { return InitialEffects; }
    /** Passive abilities always granted to avatars using this profile */
    FORCEINLINE const TArray<TSubclassOf<UGameplayAbility>>& GetPassiveAbilities() const { return PassiveAbilities; }
    /** DataTable with base stats per hero/archetype */
    FORCEINLINE const UDataTable* GetBaseStatDataTable() const { return BaseStatDataTable; }
    /** Returns experience curve for progression lookups */
    const FRealCurve* GetExperienceCurve() const;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
    /** GameplayEffect that fills all relevant attributes to max */
    TSubclassOf<UGameplayEffect> FullStatEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
    /** GameplayEffect used to set death state and/or broadcast cues */
    TSubclassOf<UGameplayEffect> DeathEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
    /** Effects applied at startup (e.g., regen, resistances) */
    TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
    /** Passive abilities to grant on init (e.g., stat watchers, HUD notifiers) */
    TArray<TSubclassOf<UGameplayAbility>> PassiveAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "Base Stats")
    /** Table mapping hero classes to their base stats and growth */
    UDataTable* BaseStatDataTable;

    UPROPERTY(EditDefaultsOnly, Category = "Level")
    /** Row name for experience curve lookup */
    FName ExperienceRowName = "ExperieneNeededToReachLevel";

    UPROPERTY(EditDefaultsOnly, Category = "Level")
    /** Curve table providing XP required per level */
    UCurveTable* ExperienceCurveTable;
};
