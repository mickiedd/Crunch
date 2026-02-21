// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CGameplayAbility.h"
#include "GAP_Launched.generated.h"

/**
 @file GAP_Launched.h
 @brief Passive/triggered Gameplay Ability for handling the launched (airborne) state.

 This ability is activated when the character is launched into the air (e.g., by
 an uppercut or explosion). It can set transient state (tags/effects), play
 airborne/knock-up animations, and restrict certain inputs until the character
 lands. It may also broadcast gameplay tags for cues or UI state.

 Replication/Prediction Notes:
 - Follows standard GAS activation and prediction. Server should authoritatively
   apply state/effects; client-side cues/animations should be predicted where safe.
*/
UCLASS()
class UGAP_Launched : public UCGameplayAbility
{
    GENERATED_BODY()
public:
    /** Default constructor for setting tags/cooldowns and flags. */
    UGAP_Launched();
    /**
     Activate launched state: apply temporary tags/effects, play montage (if any),
     and configure input gating until end conditions (e.g., landing) are met.
    */
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    /** Returns the gameplay tag used to identify this ability's activation trigger. */
    static FGameplayTag GetLauchedAbilityActiationTag();
};
