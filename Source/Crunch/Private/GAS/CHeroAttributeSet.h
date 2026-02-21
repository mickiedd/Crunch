// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CHeroAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
  	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
  	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
  	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
  	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * Attribute set for hero-specific progression and economy stats.
 * Includes Intelligence/Strength and growth, XP/Level ladders, gold, and
 * upgrade points. Replicated with OnRep hooks for UI/reactivity.
 */
UCLASS()
class UCHeroAttributeSet : public UAttributeSet
{
    GENERATED_BODY()
public:
    /** Accessors for hero attributes exposed to ASC and UI */
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, Intelligence)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, Strength)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, Experience)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, PrevLevelExperience)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, NextLevelExperience)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, Level)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, UpgradePoint)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, MaxLevel)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, MaxLevelExperience)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, Gold)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, StrengthGrowthRate)
    ATTRIBUTE_ACCESSORS(UCHeroAttributeSet, IntelligenceGrowthRate)
    virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const override;
private:
    /** Core hero stats */
    UPROPERTY(ReplicatedUsing = OnRep_Intelligence)
    FGameplayAttributeData Intelligence;

    UPROPERTY(ReplicatedUsing = OnRep_Strength)
    FGameplayAttributeData Strength;
    
    UPROPERTY(ReplicatedUsing = OnRep_Experience)
    FGameplayAttributeData Experience;

    /** Growth rates (local/cached, non-replicated attributes) */
    UPROPERTY()
    FGameplayAttributeData StrengthGrowthRate;
    
    UPROPERTY()
    FGameplayAttributeData IntelligenceGrowthRate;

    /** XP thresholds and level values */
    UPROPERTY(ReplicatedUsing = OnRep_PrevLevelExperience)
    FGameplayAttributeData PrevLevelExperience;

    UPROPERTY(ReplicatedUsing = OnRep_NextLevelExperience)
    FGameplayAttributeData NextLevelExperience;

    UPROPERTY(ReplicatedUsing = OnRep_Level)
    FGameplayAttributeData Level;
    
    UPROPERTY(ReplicatedUsing = OnRep_UpgradePoint)
    FGameplayAttributeData UpgradePoint;
    
    UPROPERTY(ReplicatedUsing = OnRep_MaxLevel)
    FGameplayAttributeData MaxLevel;

    UPROPERTY(ReplicatedUsing = OnRep_MaxLevelExperience)
    FGameplayAttributeData MaxLevelExperience;

    /** Currency */
    UPROPERTY(ReplicatedUsing = OnRep_Gold)
    FGameplayAttributeData Gold;

    /** Replication notification handlers */
    UFUNCTION()
    void OnRep_Intelligence(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_Strength(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_Experience(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_PrevLevelExperience(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_NextLevelExperience(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_Level(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_UpgradePoint(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_MaxLevel(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_MaxLevelExperience(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_Gold(const FGameplayAttributeData& OldValue);
};
