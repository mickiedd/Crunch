// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "GAS/CGameplayAbilityTypes.h"
#include "CAbilitySystemComponent.generated.h"

/**
 * Crunch-specific Ability System Component (ASC) extension.
 * Responsibilities:
 * - Initialize and grant initial abilities/effects to the avatar.
 * - Expose project-specific ability maps keyed by ECAbilityInputID.
 * - Provide helpers for attribute init and server-authoritative upgrades.
 */
UCLASS()
class UCAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()
public:
    UCAbilitySystemComponent();
    /** Initialize default attributes on owning avatar (called on init/spawn) */
    void InitializeBaseAttributes();
    /** Perform server-side initialization tasks after possession/spawn */
    void ServerSideInit();
    /** Apply an effect that restores stats to full (health/mana, etc.) */
    void ApplyFullStatEffect();
    //Get the Abilities that is unique for the avatar actor, this do not include Generic/Basic ones
    /** Returns the unique (non-basic) abilities granted to the avatar, keyed by input ID */
    const TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>>& GetAbilities() const;
    /** Whether the owner has reached maximum character level */
    bool IsAtMaxLevel() const;

    UFUNCTION(Server, Reliable, WithValidation)
    /** Server RPC: upgrade an ability identified by input ID */
    void Server_UpgradeAbilityWithID(ECAbilityInputID InputID);

    UFUNCTION(Client, Reliable)
    /** Client RPC: notify when an ability spec level has changed */
    void Client_AbilitySpecLevelUpdated(FGameplayAbilitySpecHandle Handle, int NewLevel);

private:
    /** Apply initial passive/starting effects to the avatar */
    void ApplyInitialEffects();
    /** Grant starting abilities (basic + unique) to the avatar */
    void GiveInitialAbilities();
    /** Apply a gameplay effect on the authority (server) */
    void AuthApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int Level = 1);
    /** Attribute change handlers bound to ASC delegates */
    void HealthUpdated(const FOnAttributeChangeData& ChangeData);
    void ManaUpdated(const FOnAttributeChangeData& ChangeData);
    void ExperienceUpdated(const FOnAttributeChangeData& ChangeData);

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
    /** Unique abilities (non-basic), mapped to their input bindings */
    TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>> Abilities;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
    /** Always-available/basic abilities (e.g., basic attack, confirm/cancel) */
    TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>> BasicAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Ability")
    /** Shared helper object for generic GAS operations used by abilities */
    class UPA_AbilitySystemGenerics* AbilitySystemGenerics;
};
