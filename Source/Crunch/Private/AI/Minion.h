// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CCharacter.h"
// ----------------------------------------------------------------------------
// File: Minion.h
// Purpose: Defines AMinion, a lightweight AI pawn derived from ACCharacter.
//          Supports team-based visuals (SkinMap), simple active/respawn state,
//          and a blackboard-configurable goal for behavior logic.
// Key API:
//  - SetGenericTeamId: updates team and applies team-specific skeletal mesh.
//  - IsActive / Activate: mirrors death/respawn semantics in ACCharacter.
//  - SetGoal: writes a Goal actor reference to the controller's blackboard.
// ----------------------------------------------------------------------------
#include "Minion.generated.h"

/**
 * AI pawn with team-driven appearance and a Blackboard Goal key.
 *
 * Responsibilities:
 *  - Team visuals: applies a SkeletalMesh from SkinMap keyed by FGenericTeamId.
 *  - Lifecycle: IsActive() reflects !IsDead(); Activate() performs immediate
 *    respawn via RespawnImmediately().
 *  - Behavior input: SetGoal(AActor*) writes a Goal object into the
 *    controller's UBlackboardComponent under GoalBlackboardKeyName.
 *
 * Notes:
 *  - OnRep_TeamID ensures replicated team changes also update visuals.
 *  - GoalBlackboardKeyName defaults to "Goal" and is editable per instance.
 */
UCLASS()
class AMinion : public ACCharacter
{
    GENERATED_BODY()
public:
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;

    bool IsActive() const;
    void Activate();
    virtual void SetGoal(AActor* Goal);

private:
    void PickSkinBasedOnTeamID();

    virtual void OnRep_TeamID() override;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    FName GoalBlackboardKeyName = "Goal";

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    TMap<FGenericTeamId, USkeletalMesh*> SkinMap;
};
