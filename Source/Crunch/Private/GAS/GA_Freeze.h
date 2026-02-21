// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CGameplayAbility.h"
#include "GA_Freeze.generated.h"

/**
 @file GA_Freeze.h
 @brief Gameplay Ability for ground-targeted freeze/AoE with arrival effects and gameplay cues.

 This ability uses a ground-pick targeting actor to select a point within range, plays
 targeting and cast montages, and applies a configurable damage/effect to actors within
 a radius. It also triggers gameplay cues to visualize targeting and the freezing effect.

 Responsibilities:
 - Spawn ground targeting actor and handle confirm/cancel callbacks.
 - Validate target using `TargetRange` and apply effects within `TargetingRadius`.
 - Drive targeting and cast animations via montages.
 - Emit gameplay cues for targeting feedback and freezing impact.

 Replication/Prediction Notes:
 - Standard GAS activation/prediction applies. Server should authoritatively apply
   effects; targeting UX and cues may be predicted locally for responsiveness.
*/
UCLASS()
class UGA_Freeze : public UCGameplayAbility
{
    GENERATED_BODY()
public:	
    /** Construct default values for targeting, montages, and effect classes. */
    UGA_Freeze();
    /** Activate ability: start ground targeting flow and handle confirm/cancel. */
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
    /** Radius around selected point to apply freeze effects/damage. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float TargetingRadius = 1000.f;

    /** Max distance from the caster at which a valid target point can be chosen. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float TargetRange = 2000.f;

    /** Targeting actor class used for ground point selection. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    TSubclassOf<class ATargetActor_GroundPick> TargetActorClass;

    /** Montage shown while the player is selecting a target location. */
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* FreezeTargetingMontage;

    /** Montage played when actually casting the freeze at the selected location. */
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* FreezeCastMontage;

    /** Gameplay effect applied to actors affected by the freeze (e.g., damage/slow). */
    UPROPERTY(EditDefaultsOnly, Category = "Effect")
    TSubclassOf<UGameplayEffect> DamageEffect;

    /** Gameplay cue tag for visualizing the targeting phase. */
    UPROPERTY(EditDefaultsOnly, Category = "GameplayCue")
    FGameplayTag FreezingTargetGameplayCueTag;

    /** Gameplay cue tag used when the freezing effect is applied. */
    UPROPERTY(EditDefaultsOnly, Category = "GameplayCue")
    FGameplayTag FreezingGameplayCueTag;
    
    /** Called when the targeting actor confirms a ground location. */
    UFUNCTION()
    void TargetReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

    /** Called when the targeting flow is cancelled by the player. */
    UFUNCTION()
    void TargetingCancelled(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
