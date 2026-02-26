// Behaviac AI Test Implementation for Minion
// Demonstrates Behaviac plugin integration in Crunch project

#pragma once

#include "CoreMinimal.h"
#include "AI/Minion.h"
#include "BehaviacAgent.h"
#include "BehaviacTypes.h"
#include "BehaviorTree/BehaviacBehaviorTree.h"
#include "BehaviacTestMinion.generated.h"

/**
 * Test minion using Behaviac AI instead of UE5 Behavior Tree.
 *
 * Purpose:
 *  - Compare Behaviac performance vs UE5 default BT system
 *  - Demonstrate Behaviac's 41 node types, FSM, and HTN capabilities
 *  - Validate method-based execution model (RegisterMethodHandler pattern)
 *
 * Features:
 *  - Inherits all AMinion functionality (team, respawn, GAS, goal)
 *  - Uses Behaviac for decision-making instead of ACAIController
 *  - Lambda-based method registration via RegisterMethodHandler
 *  - Full guard/patrol/combat state machine (mirrors BehaviacAINPC)
 *  - Perception-based target detection (uses AIPerceptionComponent)
 *
 * Methods Exposed to Behaviac:
 *  - FindPlayer / HasTarget / InAttackRange
 *  - MoveToTarget, PatrolToGoal, ChasePlayer, AttackTarget
 *  - UpdateAIState, StopMovement, FaceTarget
 *  - SetWalkSpeed, SetRunSpeed
 *  - MoveToLastKnownPos, LookAround, ClearLastKnownPos, ReturnToPost
 *
 * Properties Available in XML:
 *  - Health (int): Current health value
 *  - HasTarget (bool): Whether target is acquired
 *  - DistanceToTarget (float): Distance to current target
 *  - IsMoving (bool): Whether currently navigating
 *  - AIState (string): "Patrol" / "Chase" / "Combat" / "Investigate" / "ReturnToPost"
 */
UCLASS()
class ABehaviacTestMinion : public AMinion
{
	GENERATED_BODY()

public:
	ABehaviacTestMinion();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Toggle between Behaviac and UE5 BT (for comparison / fallback)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviac AI")
	bool bUseBehaviacAI = true;

	// Asset-reference approach (preferred — set in Blueprint/level)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviac AI")
	UBehaviacBehaviorTree* BehaviorTree;

	// Fallback: path-based loading (used when BehaviorTree asset is not set)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviac AI")
	FString BehaviorTreeAssetPath = TEXT("MinionTestTree");

	// ── AI tuning ──────────────────────────────────────────────────────
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|State")
	float DetectionRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|State")
	float WalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|State")
	float RunSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Guard")
	float GuardRadius = 1500.0f;

	// ── Actions (called by behavior tree) ─────────────────────────────
	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus FindPlayer();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus MoveToTarget();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus Patrol();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus UpdateAIState();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus ChasePlayer();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus AttackTarget();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus StopMovement();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus FaceTarget();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus SetWalkSpeed();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus SetRunSpeed();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus MoveToLastKnownPos();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus LookAround();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus ClearLastKnownPos();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus ReturnToPost();

	UFUNCTION(BlueprintCallable, Category = "AI|Actions")
	EBehaviacStatus PatrolToGoal();

	// ── Conditions ────────────────────────────────────────────────────
	UFUNCTION(BlueprintCallable, Category = "AI|Conditions")
	bool IsPlayerInRange();

private:
	// Behaviac AI component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Behaviac AI", meta = (AllowPrivateAccess = "true"))
	UBehaviacAgentComponent* BehaviacAgent;

	// Update Behaviac blackboard properties from game state
	void UpdateBehaviacProperties();

	// Perception-based target finding (returns true if a hostile actor was found)
	bool FindTargetViaPerception();

	// Cached references
	UPROPERTY()
	AActor* CurrentTarget;

	// Guard / patrol state (mirrors BehaviacAINPC)
	FVector GuardCenter;
	FVector LastKnownPlayerPos;
	bool bHasLastKnownPos;
	float LookAroundDir;       // +1.0 or -1.0

	float AttackRange;
	float CombatRange;
	float MoveAcceptanceRadius;

	TArray<FVector> PatrolPoints;
	int32 CurrentPatrolIndex;

	// Periodic property sync
	float PropertyUpdateInterval;
	float LastPropertyUpdateTime;

	// Debug
	int32 TickCounter;
	float DebugTimer;
};
