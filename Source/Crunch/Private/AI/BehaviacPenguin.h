// Fill out your copyright notice in the Description page of Project Settings.
// BehaviacPenguin: A wandering penguin driven by a Behaviac behavior tree.
//
// Behavior cycle (defined in PenguinWanderTree.xml):
//   1. PickWanderTarget  – choose a random reachable point near spawn
//   2. MoveToWanderTarget – walk there (Running until arrived)
//   3. StopMovement       – halt navigation
//   4. Wait 1.5 s         – pause at destination
//   5. LookAround         – snap to random yaw (natural curiosity)
//   6. Wait 3.0 s         – rest before next wander
//   Loop forever via DecoratorLoop(Count=-1)
//
// The IsMoving blackboard property is synced from velocity every Tick so that
// AnimBP_Penguin's "idle → running" transition (screenshot) fires correctly.

#pragma once

#include "CoreMinimal.h"
#include "BehaviacAnimalBase.h"
#include "BehaviacAgent.h"
#include "BehaviacTypes.h"
#include "BehaviorTree/BehaviacBehaviorTree.h"
#include "BehaviacPenguin.generated.h"

UCLASS()
class ABehaviacPenguin : public ABehaviacAnimalBase
{
	GENERATED_BODY()

public:
	ABehaviacPenguin();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ── Wander tuning ──────────────────────────────────────────────────
	/** Maximum distance from spawn the penguin will wander */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Wander")
	float WanderRadius = 600.f;

	/** Walk speed while wandering */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Wander")
	float WanderSpeed = 150.f;

	/** Acceptance radius: how close is "arrived" */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Wander")
	float WanderAcceptanceRadius = 80.f;

	// ── Actions called by the behavior tree ───────────────────────────
	/** Pick a random destination within WanderRadius of spawn.  Always succeeds. */
	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus PickWanderTarget();

	/** Walk to the chosen destination.  Returns Running while moving, Success on arrival. */
	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus MoveToWanderTarget();

	/** Stop all navigation movement.  Always succeeds. */
	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus StopMovement();

	/** Snap to a random yaw (penguin looks around curiously).  Always succeeds. */
	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus LookAround();

private:
	// Sync blackboard properties from current game state
	void UpdateBehaviacProperties();

	// Origin used for wander-radius calculations (set once at spawn)
	FVector SpawnLocation;

	// Current wander destination
	FVector WanderTarget;
	bool    bHasWanderTarget;

	// Debug tick counter
	int32 TickCounter;
};
