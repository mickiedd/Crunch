// Behaviac AI Test Implementation for Minion

#include "AI/BehaviacTestMinion.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/CGameplayAbilityTypes.h"

ABehaviacTestMinion::ABehaviacTestMinion()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Behaviac agent component
	BehaviacAgent = CreateDefaultSubobject<UBehaviacAgentComponent>(TEXT("BehaviacAgent"));
}

void ABehaviacTestMinion::BeginPlay()
{
	Super::BeginPlay();

	if (bUseBehaviacAI && BehaviacAgent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BehaviacTestMinion] Initializing Behaviac AI for %s"), *GetName());

		// Initialize properties
		BehaviacAgent->SetIntProperty(TEXT("Health"), 100);
		BehaviacAgent->SetBoolProperty(TEXT("HasTarget"), false);
		BehaviacAgent->SetFloatProperty(TEXT("DistanceToTarget"), 999999.0f);
		BehaviacAgent->SetBoolProperty(TEXT("IsMoving"), false);

		// Bind method handler
		BehaviacAgent->OnMethodCalled.AddDynamic(this, &ABehaviacTestMinion::HandleBehaviacMethod);

		// Load behavior tree
		if (!BehaviorTreeAssetPath.IsEmpty())
		{
			BehaviacAgent->LoadBehaviorTreeByPath(BehaviorTreeAssetPath);
			UE_LOG(LogTemp, Log, TEXT("[BehaviacTestMinion] Loaded behavior tree: %s"), *BehaviorTreeAssetPath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[BehaviacTestMinion] BehaviorTreeAssetPath is empty!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[BehaviacTestMinion] Using default UE5 BT system for %s"), *GetName());
	}
}

void ABehaviacTestMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUseBehaviacAI && BehaviacAgent)
	{
		// Periodically update Behaviac properties from game state
		LastPropertyUpdateTime += DeltaTime;
		if (LastPropertyUpdateTime >= PropertyUpdateInterval)
		{
			UpdateBehaviacProperties();
			LastPropertyUpdateTime = 0.0f;
		}
	}
}

void ABehaviacTestMinion::HandleBehaviacMethod(const FString& MethodName, EBehaviacStatus& OutResult)
{
	UE_LOG(LogTemp, Log, TEXT("[BehaviacTestMinion] Method called: %s"), *MethodName);

	// Condition checks
	if (MethodName == TEXT("HasTarget"))
	{
		OutResult = CheckHasTarget() ? EBehaviacStatus::Success : EBehaviacStatus::Failure;
	}
	else if (MethodName == TEXT("InAttackRange"))
	{
		OutResult = CheckInAttackRange() ? EBehaviacStatus::Success : EBehaviacStatus::Failure;
	}
	// Actions
	else if (MethodName == TEXT("MoveToTarget"))
	{
		ExecuteMoveToTarget(OutResult);
	}
	else if (MethodName == TEXT("AttackTarget"))
	{
		ExecuteAttackTarget(OutResult);
	}
	else if (MethodName == TEXT("PatrolToGoal"))
	{
		ExecutePatrolToGoal(OutResult);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[BehaviacTestMinion] Unknown method: %s"), *MethodName);
		OutResult = EBehaviacStatus::Failure;
	}
}

bool ABehaviacTestMinion::CheckHasTarget()
{
	// Get target from AI controller's perception
	if (AAIController* AIController = GetController<AAIController>())
	{
		if (UAIPerceptionComponent* PerceptionComp = AIController->GetAIPerceptionComponent())
		{
			TArray<AActor*> PerceivedActors;
			PerceptionComp->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);

			// Find first hostile actor
			for (AActor* Actor : PerceivedActors)
			{
				if (Actor && Actor != this)
				{
					CurrentTarget = Actor;
					UE_LOG(LogTemp, Log, TEXT("[BehaviacTestMinion] Target acquired: %s"), *Actor->GetName());
					return true;
				}
			}
		}
	}

	CurrentTarget = nullptr;
	return false;
}

bool ABehaviacTestMinion::CheckInAttackRange()
{
	if (!CurrentTarget)
	{
		return false;
	}

	float Distance = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());
	bool bInRange = Distance <= AttackRange;

	if (bInRange)
	{
		UE_LOG(LogTemp, Log, TEXT("[BehaviacTestMinion] Target in range! Distance: %.1f"), Distance);
	}

	return bInRange;
}

void ABehaviacTestMinion::ExecuteMoveToTarget(EBehaviacStatus& OutResult)
{
	if (!CurrentTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BehaviacTestMinion] MoveToTarget: No target!"));
		OutResult = EBehaviacStatus::Failure;
		return;
	}

	if (AAIController* AIController = GetController<AAIController>())
	{
		EPathFollowingRequestResult::Type MoveResult = AIController->MoveToActor(
			CurrentTarget,
			MoveAcceptanceRadius,
			true,  // bStopOnOverlap
			true,  // bUsePathfinding
			false, // bCanStrafe
			nullptr,
			true   // bAllowPartialPath
		);

		if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
		{
			UE_LOG(LogTemp, Log, TEXT("[BehaviacTestMinion] Moving to target: %s"), *CurrentTarget->GetName());
			OutResult = EBehaviacStatus::Running;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[BehaviacTestMinion] Move request failed!"));
			OutResult = EBehaviacStatus::Failure;
		}
	}
	else
	{
		OutResult = EBehaviacStatus::Failure;
	}
}

void ABehaviacTestMinion::ExecuteAttackTarget(EBehaviacStatus& OutResult)
{
	if (!CurrentTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BehaviacTestMinion] AttackTarget: No target!"));
		OutResult = EBehaviacStatus::Failure;
		return;
	}

	// Trigger GAS combo ability
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);
	if (ASC)
	{
		// Use BasicAttack input ID (minions use this for their attack combo)
		ASC->PressInputID(static_cast<int32>(ECAbilityInputID::BasicAttack));
		
		UE_LOG(LogTemp, Warning, TEXT("[BehaviacTestMinion] ATTACKING %s!"), *CurrentTarget->GetName());
		OutResult = EBehaviacStatus::Success;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[BehaviacTestMinion] No AbilitySystemComponent!"));
		OutResult = EBehaviacStatus::Failure;
	}
}

void ABehaviacTestMinion::ExecutePatrolToGoal(EBehaviacStatus& OutResult)
{
	// Use the Goal system inherited from AMinion
	if (AAIController* AIController = GetController<AAIController>())
	{
		// Try to get goal from blackboard (if using UE5 BT hybrid)
		// For pure Behaviac, we'll just use a simple patrol behavior
		
		UE_LOG(LogTemp, Log, TEXT("[BehaviacTestMinion] Patrolling (no specific goal)"));
		
		// Simple patrol: just stand still for now
		// In a real implementation, you'd move to waypoints or the goal actor
		OutResult = EBehaviacStatus::Success;
	}
	else
	{
		OutResult = EBehaviacStatus::Failure;
	}
}

void ABehaviacTestMinion::UpdateBehaviacProperties()
{
	if (!BehaviacAgent)
	{
		return;
	}

	// Update health
	// Note: ACCharacter has health via GAS attributes
	// For simplicity, we'll use a placeholder
	BehaviacAgent->SetIntProperty(TEXT("Health"), 100);

	// Update HasTarget
	bool bHasTarget = (CurrentTarget != nullptr);
	BehaviacAgent->SetBoolProperty(TEXT("HasTarget"), bHasTarget);

	// Update distance to target
	float Distance = 999999.0f;
	if (CurrentTarget)
	{
		Distance = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());
	}
	BehaviacAgent->SetFloatProperty(TEXT("DistanceToTarget"), Distance);

	// Update movement state
	bool bIsMoving = GetCharacterMovement() && GetCharacterMovement()->Velocity.Size() > 10.0f;
	BehaviacAgent->SetBoolProperty(TEXT("IsMoving"), bIsMoving);
}
