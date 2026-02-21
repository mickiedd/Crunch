// Behaviac AI Test Implementation for Minion
// Demonstrates Behaviac plugin integration in Crunch project

#pragma once

#include "CoreMinimal.h"
#include "AI/Minion.h"
#include "BehaviacAgent.h"
#include "BehaviacTestMinion.generated.h"

/**
 * Test minion using Behaviac AI instead of UE5 Behavior Tree.
 * 
 * Purpose:
 *  - Compare Behaviac performance vs UE5 default BT system
 *  - Demonstrate Behaviac's 41 node types, FSM, and HTN capabilities
 *  - Validate method-based execution model
 * 
 * Features:
 *  - Inherits all AMinion functionality (team, respawn, GAS, goal)
 *  - Uses Behaviac for decision-making instead of ACAIController
 *  - Method-based behavior execution via OnMethodCalled delegate
 *  - Property system for blackboard-like data sharing
 * 
 * Usage:
 *  1. Place in level or spawn from MinionBarrack
 *  2. Set BehaviorTreeAssetPath to XML tree location
 *  3. Configure bUseBehaviacAI = true
 *  4. Press Play and observe Output Log for method calls
 * 
 * Methods Exposed to Behaviac:
 *  - HasTarget: Check if perception found enemy
 *  - InAttackRange: Check distance to target
 *  - MoveToTarget: Use AI movement to target
 *  - AttackTarget: Trigger GAS combo ability
 *  - PatrolToGoal: Move to assigned goal actor
 *  - UpdateProperties: Sync health/state to Behaviac
 * 
 * Properties Available in XML:
 *  - Health (int): Current health value
 *  - HasTarget (bool): Whether target is acquired
 *  - DistanceToTarget (float): Distance to current target
 *  - IsMoving (bool): Whether currently navigating
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

	// Toggle between Behaviac and UE5 BT (for comparison)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviac AI")
	bool bUseBehaviacAI = true;

	// Path to Behaviac XML tree asset (e.g., "MinionBehaviacTree")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviac AI")
	FString BehaviorTreeAssetPath = "/Game/AI/Behaviac/MinionTestTree";

private:
	// Behaviac AI component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Behaviac AI", meta = (AllowPrivateAccess = "true"))
	UBehaviacAgentComponent* BehaviacAgent;

	// Method handler for Behaviac behavior tree
	UFUNCTION()
	void HandleBehaviacMethod(const FString& MethodName, EBehaviacStatus& OutResult);

	// Behavior method implementations
	bool CheckHasTarget();
	bool CheckInAttackRange();
	void ExecuteMoveToTarget(EBehaviacStatus& OutResult);
	void ExecuteAttackTarget(EBehaviacStatus& OutResult);
	void ExecutePatrolToGoal(EBehaviacStatus& OutResult);
	
	// Update Behaviac properties from game state
	void UpdateBehaviacProperties();

	// Cached references
	UPROPERTY()
	AActor* CurrentTarget;
	
	float AttackRange = 200.0f;
	float MoveAcceptanceRadius = 100.0f;
	float PropertyUpdateInterval = 0.2f;
	float LastPropertyUpdateTime = 0.0f;
};
