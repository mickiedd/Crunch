// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CGameplayAbility.h"
#include "GA_Shoot.generated.h"

/**
 * Gameplay Ability: Shoot
 * Handles ranged shooting with projectile spawning, hit effects, and optional
 * continuous fire while input is held. Manages aim target tracking and range
 * validation, and sends start/stop events for montage sections.
 */
UCLASS()
class UGA_Shoot : public UCGameplayAbility
{
    GENERATED_BODY()
public:
    /** Configure tags, costs, and montage defaults */
    UGA_Shoot();
    /** Start shooting: play montage, begin target check, and fire if valid */
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    /** Stop continuous fire when input released */
    virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
    /** Cleanup timers/VFX and replicate end */
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
private:
    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    /** GameplayEffect applied on projectile hit (e.g., damage/knockback) */
    TSubclassOf<UGameplayEffect> ProjectileHitEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    /** Initial projectile speed */
    float ShootProjectileSpeed = 2000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    /** Max projectile travel distance */
    float ShootProjectileRange = 3000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    /** Projectile actor to spawn when firing */
    TSubclassOf<class AProjectileActor> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, Category = "Anim")
    /** Montage played when shooting */
    UAnimMontage* ShootMontage;

    /** Tag for gameplay events related to shooting */
    static FGameplayTag GetShootTag();

    UFUNCTION()
    /** Event handler to begin continuous firing */
    void StartShooting(FGameplayEventData Payload);

    UFUNCTION()
    /** Event handler to end continuous firing */
    void StopShooting(FGameplayEventData Payload);

    UFUNCTION()
    /** Actually spawn and launch the projectile */
    void ShootProjectile(FGameplayEventData Payload);

    /** Cached current aim target if valid */
    AActor* GetAimTargetIfValid() const;

    UPROPERTY()
    AActor* AimTarget;

    UPROPERTY()
    UAbilitySystemComponent* AimTargetAbilitySystemComponent;

    FTimerHandle AimTargetCheckTimerHandle;

    /** Periodically update aim target during held fire */
    void FindAimTarget();

    UPROPERTY(EditDefaultsOnly, Category = "Target")
    /** How often to check for a new target while holding fire */
    float AimTargetCheckTimeInterval = 0.1f;

    void StartAimTargetCheckTimer();
    void StopAimTargetCheckTimer();

    /** Target validation helpers */
    bool HasValidTarget() const;
    bool IsTargetInRange() const;

    /** Update on target dead tag changes to stop firing */
    void TargetDeadTagUpdated(const FGameplayTag Tag, int32 NewCount);
};
