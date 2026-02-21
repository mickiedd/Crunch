// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CGameplayAbility.h"
#include "GA_Dash.generated.h"

/**
 @file GA_Dash.h
 @brief Gameplay Ability for short, directional dash with target acquisition and impact effects.

 This ability performs a quick dash, optionally snapping toward a detected target within a radius.
 It plays a dash montage, applies a temporary dash gameplay effect, and can push/damage hit targets.
 Target acquisition is handled via a targeting actor that collects nearby valid actors.

 Responsibilities:
 - Start/stop dash state and apply/remove temporary dash effect.
 - Play montage and emit/handle dash start events via `GetDashStartTag()`.
 - Query nearby targets using `ATargetActor_Around` and apply impact effects/impulses.
 - Optionally nudge forward input during dash to maintain momentum.

 Replication/Prediction Notes:
 - Activation follows standard GAS semantics. Dash movement/effects should be applied on server
   with prediction window for responsiveness. Local gameplay cue tag can be used for client VFX/SFX.
*/
UCLASS()
class UGA_Dash : public UCGameplayAbility
{
    GENERATED_BODY()
public:
    /**
     Begin the dash ability: plays montage, applies dash effect, sets up targeting and input push.
    */
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override; private:
    /**
     End the dash ability: clears timers/effects and restores normal movement state.
    */
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    /** Gameplay tag that marks the start of the dash (for montages/cues). */
    static FGameplayTag GetDashStartTag();
private:
    /** Dash montage to play on activation. */
    UPROPERTY(EditDefaultsOnly, Category = "Anim")
    UAnimMontage* DashMontage;

    /** Radius to search for dash targets around the owner. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float TargetDetectionRadius = 300.f;

    /** Local-only gameplay cue for VFX/SFX while dashing. */
    UPROPERTY(EditDefaultsOnly, Category = "GameplayCue")
    FGameplayTag LocalGameplayCueTag;

    /** Socket on the target used for attach or aim alignment during dash. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    FName TargetActorAttachSocketName = "TargetDashCenter";

    /** Targeting actor class to gather actors around the owner for dash interaction. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    TSubclassOf<class ATargetActor_Around> TargetActorClass;

    /** Forward impulse applied to a hit target during dash. */
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    float TargetHitPushSpeed = 3000.f;

    /** Optional damage effect applied to hit targets. */
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TSubclassOf<UGameplayEffect> DamageEffect;

    /** Temporary dash effect (e.g., movement buff, invuln frames) applied on start. */
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TSubclassOf<UGameplayEffect> DashEffect;
    
    /** Handle to the active dash gameplay effect for later removal. */
    FActiveGameplayEffectHandle DashEffectHandle;

    /** Timer handle for periodically pushing forward input while dashing. */
    FTimerHandle PushForwardInputTimerHandle;

    /** Apply a small forward input to keep momentum during dash. */
    void PushForward();
    /** Cached movement component of the owning character. */
    class UCharacterMovementComponent* OwnerCharacterMovementComponent;

    /** Receives dash start gameplay event; may initiate movement/impulse. */
    UFUNCTION()
    void StartDash(FGameplayEventData Payload);

    /** Callback receiving target data gathered during dash to apply effects. */
    UFUNCTION()
    void TargetReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
