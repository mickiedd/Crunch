// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h"
// ----------------------------------------------------------------------------
// File: CAIController.h
// Purpose: AI controller managing perception-driven targeting and integrating
//          with GAS gameplay tags (Dead/Stun) to pause/resume AI logic and
//          senses. Maintains a blackboard Target and mirrors team affiliation
//          from the possessed pawn.
// Key API:
//  - TargetBlackboardKeyName: name of blackboard key storing current target.
//  - BehaviorTree: the BT asset to run on BeginPlay.
//  - Perception components: UAIPerceptionComponent + UAISenseConfig_Sight.
//  - Tag handlers: PawnDeadTagUpdated / PawnStunTagUpdated.
// ----------------------------------------------------------------------------
#include "CAIController.generated.h"

/**
 * Controller that:
 *  - Runs the assigned BehaviorTree.
 *  - Uses AIPerception (Sight) to set/clear a blackboard Target key.
 *  - Mirrors the pawn's team ID (IGenericTeamAgentInterface).
 *  - Reacts to GAS Dead/Stun tags to stop/start brain and toggle senses.
 *
 * Behavior highlights:
 *  - On sensing a hostile and no target: sets Target.
 *  - On forgetting: switches to next perceived hostile actor.
 *  - On Dead tag applied: stop logic, disable senses, mark dead.
 *  - On Stun tag applied: stop logic (unless dead). On removal: resume.
 */
UCLASS()
class ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACAIController();

	virtual void OnPossess(APawn* NewPawn) override;
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI Behavior")
	FName TargetBlackboardKeyName = "Target";

	UPROPERTY(EditDefaultsOnly, Category = "AI Behavior")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);
	UFUNCTION()
	void TargetForgotten(AActor* ForgottenActor);

	const UObject* GetCurrentTarget() const;
	void SetCurrentTarget(AActor* NewTarget);

	AActor* GetNextPerceivedActor() const;

	void ForgetActorIfDead(AActor* ActorToForget);

	void ClearAndDisableAllSenses();
	void EnableAllSenses();

	void PawnDeadTagUpdated(const FGameplayTag Tag, int32 Count);
	void PawnStunTagUpdated(const FGameplayTag Tag, int32 Count);

	bool bIsPawnDead = false;
};
