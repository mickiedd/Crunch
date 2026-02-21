// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CGameplayAbility.h"
#include "GA_Blink.generated.h"

/**
 @file GA_Blink.h
 @brief Gameplay Ability for short-range teleportation (blink) with ground targeting and impact effects.

 This ability lets the owning character select a ground position within range and teleport
 to it, optionally playing targeting and teleport montages. Upon arrival, the ability can
 apply effects, such as damage or knockback, to nearby targets using configurable parameters.

 Responsibilities:
 - Spawn and manage a ground-pick targeting actor to acquire a destination.
 - Validate destination against cast range and target area radius.
 - Play targeting/teleport montages and handle related gameplay events.
 - Apply arrival effects (e.g., damage, push/knockback) after teleporting.

 Replication/Prediction Notes:
 - Follows standard GAS ability activation and prediction. Teleportation should execute
   under server authority while respecting prediction windows for responsiveness.

 Related Types/Tags:
 - Uses `ATargetActor_GroundPick` for destination selection.
 - `GetTeleportationTag()` identifies gameplay events/cues associated with teleportation.
*/
UCLASS()
class UGA_Blink : public UCGameplayAbility
{
    GENERATED_BODY()
public:
    /** Construct default values for animations, targeting, and effect parameters. */
    UGA_Blink();
    /**
     Activate the blink ability.
     Spawns the ground targeting actor, plays targeting montage, and awaits selection
     callbacks to proceed with teleportation and arrival effects.
    */
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
    /** Gameplay tag used to identify teleportation-related events/cues. */
    static FGameplayTag GetTeleportationTag();
private:
    /** Montage played while aiming/selecting a destination. */
    UPROPERTY(EditDefaultsOnly, Category = "Anim")
    UAnimMontage* TargetingMontage;
    
    /** Montage played when executing the teleport. */
    UPROPERTY(EditDefaultsOnly, Category = "Anim")
    UAnimMontage* TeleportMontage;

    /** Targeting actor class used to select a valid ground location. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    TSubclassOf<class ATargetActor_GroundPick> GroundPickTargetActorClass;

    /** Gameplay effect applied to nearby actors upon landing (optional). */
    UPROPERTY(EditDefaultsOnly, Category = "Effect")
    TSubclassOf<UGameplayEffect> DamageEffect;

    /** Speed/impulse applied to targets at the landing area to push them outward. */
    UPROPERTY(EditDefaultsOnly, Category = "Effect")
    float BlinkLandTargetPushSpeed = 5000.f;

    /** Radius around the landing position to consider for applying arrival effects. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float TargetAreaRadius = 1000;

    /** Maximum distance the blink can cover from the caster's current position. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float BlinkCastRange = 3000.f;

    /** Callback when the ground targeting actor confirms a selection. */
    UFUNCTION()
    void GroundPickTargetReceived(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

    /** Callback when the ground targeting is cancelled by the player. */
    UFUNCTION()
    void GroundPickCancelled(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

    /** Executes the teleport and applies arrival effects based on the selected target data. */
    UFUNCTION()
    void Teleport(FGameplayEventData Payload);

    /** Cached target data for the chosen blink destination. */
    FGameplayAbilityTargetDataHandle BlinkTargetDataHandle;
};
