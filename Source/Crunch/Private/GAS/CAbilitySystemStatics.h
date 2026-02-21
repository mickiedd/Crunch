// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CAbilitySystemStatics.generated.h"

class UGameplayAbility;
struct FGameplayAbilitySpec;
class UAbilitySystemComponent;

/**
 * Centralized static helpers for Crunch GAS:
 * - Common gameplay tags (state/cues/attributes) accessors
 * - Cooldown and cost queries (static and instance-based)
 * - Convenience checks for hero/dead and ability level caps
 * - Gameplay cue sender for local cosmetic feedback
 */
UCLASS()
class UCAbilitySystemStatics : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    /** Common tag accessors */
    static FGameplayTag GetBasicAttackAbilityTag();
    static FGameplayTag GetBasicAttackInputPressedTag();
    static FGameplayTag GetBasicAttackInputReleasedTag();
    static FGameplayTag GetDeadStatTag();
    static FGameplayTag GetStunStatTag();
    static FGameplayTag GetAimStatTag();
    static FGameplayTag GetFocusStatTag();
    static FGameplayTag GetCameraShakeGameplayCueTag();
    static FGameplayTag GetHealthFullStatTag();
    static FGameplayTag GetHealthEmptyStatTag();
    static FGameplayTag GetManaFullStatTag();
    static FGameplayTag GetManaEmptyStatTag();
    static FGameplayTag GetHeroRoleTag();
    static FGameplayTag GetExperienceAttributeTag();
    static FGameplayTag GetGoldAttributeTag();
    static FGameplayTag GetCrosshairTag();
    static FGameplayTag GetTargetUpdatedTag();
    static FGameplayTag GetGenericDamagePointTag();
    static FGameplayTag GetGenericTargetPointTag();

    /** Actor/ability state helpers */
    static bool IsActorDead(const AActor* ActorToCheck);
    static bool IsHero(const AActor* ActorToCheck);
    static bool ActorHasTag(const AActor* ActorToCheck, const FGameplayTag& Tag);
    static bool IsAbilityAtMaxLevel(const FGameplayAbilitySpec& Spec);

    /** Static costs/cooldowns pulled from CDO or data assets */
    static float GetStaticCooldownDurationForAbility(const UGameplayAbility* Ability);
    static float GetStaticCostForAbility(const UGameplayAbility* Ability);

    /** Instance-based checks against a specific ASC */
    static bool CheckAbilityCost(const FGameplayAbilitySpec& AbilitySpec, const UAbilitySystemComponent& ASC);
    static bool CheckAbilityCostStatic(const UGameplayAbility* AbilityCDO, const UAbilitySystemComponent& ASC);
    static float GetManaCostFor(const UGameplayAbility* AbilityCDO, const UAbilitySystemComponent& ASC, int AbilityLevel);
    static float GetCooldownDurationFor(const UGameplayAbility* AbilityCDO, const UAbilitySystemComponent& ASC, int AbilityLevel);
    static float GetCooldownRemainingFor(const UGameplayAbility* AbilityCDO, const UAbilitySystemComponent& ASC);
    /** Fire a local gameplay cue with optional hit result context */
    static void SendLocalGameplayCue(AActor* CueTargetActor, const FHitResult& HitResult, const FGameplayTag& GameplayCueTag);
};
