// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TargetActor_GroundPick.generated.h"

/**
 * Target Actor: Ground Picker
 * Projects a decal to select a point on the ground within range, with an area
 * radius for AoE preview. Reports target data back to the owning ability.
 * Options allow filtering friendlies/enemies and toggling debug.
 */
UCLASS()
class ATargetActor_GroundPick : public AGameplayAbilityTargetActor
{
    GENERATED_BODY()
public:
    /** Set up components and defaults */
    ATargetActor_GroundPick();

    /** Adjust indicator radius at runtime */
    void SetTargetAreaRadius(float NewRadius);
    FORCEINLINE void SetTargetTraceRange(float NewRange) { TargetTraceRange = NewRange; }
    virtual void ConfirmTargetingAndContinue() override;
    /** Configure whether to include friendly/enemy actors in results */
    void SetTargetOptions(bool bTargetFriendly, bool bTargetEnenmy = true);
    FORCEINLINE void SetShouldDrawDebug(bool bDrawDebug) { bShouldDrawDebug = bDrawDebug; }

private:
    UPROPERTY(VisibleDefaultsOnly, Category = "Visual")
    class UDecalComponent* DecalComp;

    /** Filters controlling which actors are considered valid */
    bool bShouldTargetEnemy = true;
    bool bShouldTargetFriendly = false;

    virtual void Tick(float DeltaTime) override;

    /** Trace under cursor/crosshair to find ground point */
    FVector GetTargetPoint() const;
    
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float TargetAreaRadius = 300.f;

    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    float TargetTraceRange = 2000.f;

    /** Enables debug drawing for traces/decals */
    bool bShouldDrawDebug = false;
};
