// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GenericTeamAgentInterface.h"
#include "TargetActor_Around.generated.h"

/**
 * Target Actor: Around (AoE proximity detector)
 * Spawns a sphere around the owner to detect overlapping actors and reports
 * them as target data. Replicates team, detection radius, and local cue tag.
 */
UCLASS()
class ATargetActor_Around : public AGameplayAbilityTargetActor, public IGenericTeamAgentInterface
{
    GENERATED_BODY()
public:
    /** Construct components and set defaults */
    ATargetActor_Around();
    /** Set detection radius, team filter, and optional local cue */
    void ConfigureDetection(float DetectionRadius, const FGenericTeamId& InTeamId, const FGameplayTag& InLocalGameplayCueTag);

    /** Assigns Team Agent to given TeamID */
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID);
    
    /** Retrieve team identifier in form of FGenericTeamId */
    FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const { return TeamId; }
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
    /** Team filter used to validate overlaps (replicated) */
    UPROPERTY(Replicated)
    FGenericTeamId TeamId;

    UPROPERTY(VisibleDefaultsOnly, Category = "Comp")
    class USceneComponent* RootComp;

    UPROPERTY(VisibleDefaultsOnly, Category = "Targeting")
    class USphereComponent* DetectionSphere;

    /** Radius for detection sphere (replicated via OnRep) */
    UPROPERTY(ReplicatedUsing = OnRep_TargetDetectionRadiusReplicated)
    float TargetDetectionRadius;

    UFUNCTION()
    void OnRep_TargetDetectionRadiusReplicated();

    /** Optional local cue tag to broadcast when targets are detected (replicated) */
    UPROPERTY(Replicated)
    FGameplayTag LocalGameplayCueTag;

    UFUNCTION()
    void ActorInDetectionRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
