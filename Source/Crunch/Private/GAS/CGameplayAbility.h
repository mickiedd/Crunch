// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GenericTeamAgentInterface.h"
#include "CGameplayAbility.generated.h"

/**
 * Base class for all Crunch gameplay abilities.
 * Adds helpers for targeting, pushing actors, montage control, team attitude
 * checks, and debug drawing toggles.
 */
UCLASS()
class UCGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()
public:
    UCGameplayAbility();
    /** Enforce tag/cost/cooldown gates; optionally fill relevant tags */
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
protected:
    /** Single-line aim to acquire a target within distance with team filter */
    AActor* GetAimTarget(float AimDistance, ETeamAttitude::Type TeamAttitude) const;
    /** Retrieve owning avatar's anim instance */
    class UAnimInstance* GetOwnerAnimInstance() const;
    /** Expand target data to hit results using sweep for proximity-based effects */
    TArray<FHitResult> GetHitResultFromSweepLocationTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle, float SphereSweepRadius = 30.f, ETeamAttitude::Type TargetTeam = ETeamAttitude::Hostile, bool bDrawDebug = false, bool bIgnoreSelf = true) const;
    UFUNCTION()
    FORCEINLINE bool ShouldDrawDebug() const { return bShouldDrawDebug; }
    /** Apply impulse from owner to self/targets in various patterns */
    void PushSelf(const FVector& PushVel);
    void PushTarget(AActor* Target, const FVector& PushVel);
    void PushTargets(const TArray<AActor*>& Targets, const FVector& PushVel);
    void PushTargets(const FGameplayAbilityTargetDataHandle& TargetDataHandle, const FVector& PushVel);
    void PushTargetsFromOwnerLocation(const TArray<AActor*>& Targets, float PushSpeed);
    void PushTargetsFromOwnerLocation(const FGameplayAbilityTargetDataHandle& TargetDataHandle, float PushSpeed);
    void PushTargetsFromLocation(const FGameplayAbilityTargetDataHandle& TargetDataHandle, const FVector& FromLocation, float PushSpeed);
    void PushTargetsFromLocation(const TArray<AActor*>& Targets, const FVector& FromLocation, float PushSpeed);
    /** Local montage play/stop convenience */
    void PlayMontageLocally(UAnimMontage* MontageToPlay);
    void StopMontageAfterCurrentSection(UAnimMontage* MontageToStop);
    /** Team helpers for filtering targets */
    FGenericTeamId GetOwnerTeamId() const;

    bool IsActorTeamAttitudeIs(const AActor* OtherActor, ETeamAttitude::Type TeamAttitude) const;

    /** Cached accessors/utilities */
    ACharacter* GetOwningAvatarCharacter();
    /** Convenience to apply GE to actor hit by a trace */
    void ApplyGameplayEffectToHitResultActor(const FHitResult& HitResult, TSubclassOf<UGameplayEffect> GameplayEffect, int Level = 1);
    /** Send local gameplay event (e.g., for montage notify/cues) */
    void SendLocalGameplayEvent(const FGameplayTag& EventTag, const FGameplayEventData& EventData);
private:
    UPROPERTY(EditDefaultsOnly, Category = "Debug")
    /** Enables extra debug visuals/traces when true */
    bool bShouldDrawDebug = false;

    UPROPERTY()
    /** Cached avatar character for quick access */
    class ACharacter* AvatarCharacter;
};
