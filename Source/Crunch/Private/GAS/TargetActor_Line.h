// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GenericTeamAgentInterface.h"
#include "TargetActor_Line.generated.h"

/**
 * Target Actor: Line Trace + Cylinder Detection
 * Continuously traces forward from the avatar to acquire a single target in a
 * line, with a cylinder radius for permissive detection (e.g., laser aim).
 * Replicates range/radius/team and reports targets at a fixed interval.
 */
UCLASS()
class ATargetActor_Line : public AGameplayAbilityTargetActor, public IGenericTeamAgentInterface
{
    GENERATED_BODY()
public:
    /** Construct components and set defaults */
    ATargetActor_Line();
    /** Configure detection parameters and debug settings */
    void ConfigureTargetSetting(
        float NewTargetRange,
        float NewDetectionCylinderRadius,
        float NewTargetingInterval,
        FGenericTeamId OwnerTeamId,
        bool bShouldDrawDebug
    );

    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
    
    /** Retrieve team identifier in form of FGenericTeamId */
    FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void StartTargeting(UGameplayAbility* Ability) override;
    virtual void Tick(float DeltaTime) override;
    virtual void BeginDestroy() override;
private:
    /** Max trace distance for line targeting (replicated for consistency) */
    UPROPERTY(Replicated)
    float TargetRange;

    /** Cylinder radius around line trace used to accept targets (replicated) */
    UPROPERTY(Replicated)
    float DetectionCylinderRadius;

    UPROPERTY()
    float TargetingInterval;

    /** Owner team used for friendly/foe filtering (replicated) */
    UPROPERTY(Replicated)
    FGenericTeamId TeamId;

    UPROPERTY()
    bool bDrawDebug;

    /** Cached pointer to avatar for trace origin (replicated) */
    UPROPERTY(Replicated)
    const AActor* AvatarActor;

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    FName LazerFXLengthParamName = "Length";

    UPROPERTY(VisibleDefaultsOnly, Category = "Component")
    class USceneComponent* RootComp;

    UPROPERTY(VisibleDefaultsOnly, Category = "Component")
    class UNiagaraComponent* LazerVFX;

    UPROPERTY(VisibleDefaultsOnly, Category = "Component")
    class USphereComponent* TargetEndDetectionSphere;

    FTimerHandle PeoridicalTargetingTimerHandle;

    /** Perform trace and send target data to ability system */
    void DoTargetCheckAndReport();

    /** Update VFX and debug line to reflect current trace */
    void UpdateTargetTrace();

    /** Team and validity filter for candidate target */
    bool ShouldReportActorAsTarget(const AActor* ActorToCheck) const;
};
