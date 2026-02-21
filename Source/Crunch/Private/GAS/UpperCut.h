// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CGameplayAbility.h"
#include "GAS/CGameplayAbilityTypes.h"
#include "UpperCut.generated.h"

/**
 @file UpperCut.h
 @brief Gameplay Ability for performing an upper‑cut attack with combo support and launch behavior.

 This ability implements an upper‑cut melee attack that can be chained as part of a combo
 and can optionally launch hit targets into the air. Damage and effects are resolved through
 gameplay effect definitions keyed by combo step. The ability also reacts to gameplay events
 (combo change/commit/damage) fired during the montage, and can apply a dedicated launch
 damage effect with configurable launch speeds.

 Responsibilities:
 - Play an upper‑cut montage and drive combo progression via gameplay events.
 - Look up combo‑specific damage effects using `ComboDamageMap`.
 - Apply a separate launch effect and vertical velocity for launch phases.
 - Expose tuning knobs for designers via editable properties.

 Replication/Prediction Notes:
 - As a `UCGameplayAbility`, activation is expected to follow standard GAS activation and
   prediction rules. Any gameplay effects applied should be configured appropriately for
   server authority and prediction windows.

 Related Types/Tags:
 - `FGenericDamgeEffectDef` to define damage/effect data per combo step.
 - `GetUpperCutLaunchTag()` returns the gameplay tag used to identify the launch phase/event.
*/
UCLASS()
class UUpperCut : public UCGameplayAbility
{
    GENERATED_BODY()
public:	
    /**
     Activate the upper‑cut ability.
     Plays the montage, binds to combo gameplay events, and applies combo/launch effects
     when corresponding events are received.
    */
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    /** Construct defaults for montage, speeds, and maps. */
    UUpperCut();
private:

    /**
     Map of combo section name -> damage/effect definition.
     Used to pick the appropriate damage/effect based on the current combo section
     or `NextComboName` when advancing the combo.
    */
    UPROPERTY(EditDefaultsOnly, Category = "Combo")
    TMap<FName, FGenericDamgeEffectDef> ComboDamageMap;

    /** Gameplay effect class applied when the upper‑cut launches the target. */
    UPROPERTY(EditDefaultsOnly, Category = "Launch")
    TSubclassOf<UGameplayEffect> LaunchDamageEffect;

    /** Initial upward launch speed applied to targets during the launch window. */
    UPROPERTY(EditDefaultsOnly, Category = "Launch")
    float UpperCutLaunchSpeed = 1000.f;

    /** Additional hold/air‑control speed used while holding combo into the next section. */
    UPROPERTY(EditDefaultsOnly, Category = "Launch")
    float UpperComboHoldSpeed= 100.f;

    /** Montage that drives the upper‑cut attack and emits combo gameplay events. */
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* UpperCutMontage;

    /** Gameplay tag that marks the launch phase/event for this ability. */
    static FGameplayTag GetUpperCutLaunchTag();

    /** Retrieve the damage/effect definition for the current combo step, if any. */
    const FGenericDamgeEffectDef* GetDamageEffectDefForCurrentCombo() const;

    /** Handle the launch event from the montage or gameplay cue; applies launch effect/velocity. */
    UFUNCTION()
    void StartLaunching(FGameplayEventData EventData);

    /** Handle a combo change event (e.g., section change) and update internal combo state. */
    UFUNCTION()
    void HandleComboChangeEvent(FGameplayEventData EventData);

    /** Handle a combo commit event indicating the current combo section is locked in. */
    UFUNCTION()
    void HandleComboCommitEvent(FGameplayEventData EventData);

    /** Handle a combo damage event; applies the appropriate damage effect for this step. */
    UFUNCTION()
    void HandleComboDamageEvent(FGameplayEventData EventData);

    /** Name of the next combo section to transition to when chaining the upper‑cut. */
    FName NextComboName;
};
