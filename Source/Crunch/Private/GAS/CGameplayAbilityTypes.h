// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "CGameplayAbilityTypes.generated.h"

/**
 * Input mapping for abilities and core actions. Used to bind player input to
 * specific ability slots and generic confirm/cancel operations.
 */
UENUM(BlueprintType)
enum class ECAbilityInputID : uint8
{
	None						UMETA(DisplayName="None"),
	BasicAttack					UMETA(DisplayName="Basic Attack"),
	Aim							UMETA(DisplayName="Aim"),
	AbilityOne					UMETA(DisplayName="Ability One"),
	AbilityTwo					UMETA(DisplayName="Ability Two"),
	AbilityThree					UMETA(DisplayName="Ability Three"),
	AbilityFour					UMETA(DisplayName="Ability Four"),
	AbilityFive					UMETA(DisplayName="Ability Five"),
	AbilitySix					UMETA(DisplayName="Ability Six"),
	Confirm						UMETA(DisplayName="Confirm"),
	Cancel						UMETA(DisplayName="Cancel")
};

/**
 * Lightweight definition for a generic damage effect with optional push.
 * Allows abilities to reference a GE class alongside a knockback vector.
 */
USTRUCT(BlueprintType)
struct FGenericDamgeEffectDef
{
    GENERATED_BODY()

public:
    FGenericDamgeEffectDef();

    /** GameplayEffect to apply on hit (damage, debuff, etc.) */
    UPROPERTY(EditAnywhere)
    TSubclassOf<UGameplayEffect> DamageEffect;

    /** Push direction/strength to apply when effect triggers */
    UPROPERTY(EditAnywhere)
    FVector PushVelocity;
};

/**
 * Base stat row used to initialize hero classes from a data table.
 * Provides starting attributes and per-level growth rates.
 */
USTRUCT(BlueprintType)
struct FHeroBaseStats : public FTableRowBase
{
    GENERATED_BODY()
public:
    FHeroBaseStats();

    /** Actor class for this hero/archetype */
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> Class;

    /** Primary attributes at level 1 */
    UPROPERTY(EditAnywhere)
    float Strength;

    UPROPERTY(EditAnywhere)
    float Intelligence;
	
    /** Growth added per level */
    UPROPERTY(EditAnywhere)
    float StrengthGrowthRate;

    UPROPERTY(EditAnywhere)
    float IntelligenceGrowthRate;

    /** Base combat/resource stats at level 1 */
    UPROPERTY(EditAnywhere)
    float BaseMaxHealth;

    UPROPERTY(EditAnywhere)
    float BaseMaxMana;

    UPROPERTY(EditAnywhere)
    float BaseAttackDamage;

    UPROPERTY(EditAnywhere)
    float BaseArmor;

    UPROPERTY(EditAnywhere)
    float BaseMoveSpeed;
};
