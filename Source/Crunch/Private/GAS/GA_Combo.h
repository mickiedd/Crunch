// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CGameplayAbility.h"
#include "GA_Combo.generated.h"

/**
 @file GA_Combo.h
 @brief Gameplay Ability implementing a chained melee combo with input timing and damage mapping.

 This ability plays a combo montage and listens for input presses and gameplay events to
 advance through combo sections. Each section can apply a different damage effect based on
 a name key, with a default fallback. It emits combo change/start/end tags to coordinate
 with other systems and montage notifies.

 Responsibilities:
 - Wait for input press windows to chain into the next combo section.
 - Apply damage effects per section using `DamageEffectMap` or `DefaultDamageEffect`.
 - Broadcast combo state changes via gameplay tags for UI/FX or other abilities.

 Replication/Prediction Notes:
 - Follows GAS activation/prediction rules. Damage effects should be applied on server with
   appropriate prediction windows; input waits should be locally predicted where possible.
*/
UCLASS()
class UGA_Combo : public UCGameplayAbility
{
    GENERATED_BODY()
    
public:
    /** Construct default values for montage and damage mapping. */
    UGA_Combo();
    /** Activate the combo ability and start listening for input and montage events. */
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    /** Gameplay tag broadcast when the combo section changes. */
    static FGameplayTag GetComboChangedEventTag();
    /** Gameplay tag broadcast when a combo change window ends. */
    static FGameplayTag GetComboChangedEventEndTag();
    /** Gameplay tag used when the combo targets should be evaluated/damaged. */
    static FGameplayTag GetComboTargetEventTag();

private:
    /** Set up a wait task for the next combo input window. */
    void SetupWaitComboInputPress();

    /** Input press handler fired by the wait task; advances combo state if valid. */
    UFUNCTION()
    void HandleInputPress(float TimeWaited);

    /** Attempt to commit to the next combo section and apply relevant effects. */
    void TryCommitCombo();

    /** Default damage effect used when no section-specific override is found. */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect")
    TSubclassOf<UGameplayEffect> DefaultDamageEffect;

    /** Per-combo-section name -> damage effect override. */
    UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect")
    TMap<FName, TSubclassOf<UGameplayEffect>> DamageEffectMap;

    /** Resolve the damage effect to use for the current combo section. */
    TSubclassOf<UGameplayEffect> GetDamageEffectForCurrentCombo() const;

    /** Montage that drives the combo and emits gameplay events. */
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* ComboMontage;

    /** Receive and process combo section change events from montage/gameplay cues. */
    UFUNCTION()
    void ComboChangedEventReceived(FGameplayEventData Data);

    /** Apply damage effect(s) to targets when the combo damage event is fired. */
    UFUNCTION()
    void DoDamage(FGameplayEventData Data);

    /** Name of the next combo section to transition into. */
    FName NextComboName;
};
