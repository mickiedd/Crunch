// Fill out your copyright notice in the Description page of Project Settings.
// BehaviacPenguin — Wandering penguin driven by Behaviac behavior tree.
// See BehaviacPenguin.h for the full behavior description.

#include "AI/BehaviacPenguin.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviacAgent.h"

// ============================================================
// Constructor
// ============================================================

ABehaviacPenguin::ABehaviacPenguin()
{
	PrimaryActorTick.bCanEverTick = true;

	// The base class (ABehaviacAnimalBase) creates the BehaviacAgent
	// subobject, so do not recreate it here.  Just configure it if present.
	// Create Behaviac agent component
	BehaviacAgent = CreateDefaultSubobject<UBehaviacAgentComponent>(TEXT("BehaviacAgent"));
	// Manually tick from our own Tick() for ordering control
	BehaviacAgent->bAutoTick = false;

	// Defaults
	WanderRadius          = 600.f;
	WanderSpeed           = 150.f;
	WanderAcceptanceRadius = 80.f;
	bHasWanderTarget      = false;
	TickCounter           = 0;
	SpawnLocation         = FVector::ZeroVector;
	WanderTarget          = FVector::ZeroVector;

	if (BehaviacAgent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BehaviacPenguin] %s %p: BehaviacAgent component created"), *GetName(), this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[BehaviacPenguin] %s: failed to create BehaviacAgent component!"), *GetName());
	}
}

// ============================================================
// BeginPlay
// ============================================================

void ABehaviacPenguin::BeginPlay()
{

	Super::BeginPlay();

	// ── 0. Recover BehaviacAgent if Blueprint serialization nulled it ──
	// CreateDefaultSubobject sets BehaviacAgent in the constructor, but
	// Blueprint property deserialization runs after construction and can
	// overwrite the UPROPERTY with the Blueprint CDO's stored null value.
	// FindComponentByClass retrieves the already-created component from
	// the actor's component list, restoring the pointer before we use it.
	if (!BehaviacAgent)
	{
		BehaviacAgent = FindComponentByClass<UBehaviacAgentComponent>();
		if (BehaviacAgent)
		{
			UE_LOG(LogTemp, Warning, TEXT("[BehaviacPenguin] %s %p: BehaviacAgent pointer recovered via FindComponentByClass (Blueprint CDO deserialization had nulled it)"), *GetName(), this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[BehaviacPenguin] %s %p: BehaviacAgent component truly missing — cannot run behavior tree"), *GetName(), this);
			return;
		}
	}

	// ── 1. Record spawn origin for wander-radius calculations ──────────
	SpawnLocation = GetActorLocation();

	// ── 2. Register behavior-tree method handlers ──────────────────────
	//      (must happen before the tree is loaded so the first tick can
	//       resolve method names immediately)
	if (BehaviacAgent)
	{
		BehaviacAgent->RegisterMethodHandler(TEXT("PickWanderTarget"),   [this]() { return PickWanderTarget();   });
		BehaviacAgent->RegisterMethodHandler(TEXT("MoveToWanderTarget"), [this]() { return MoveToWanderTarget(); });
		BehaviacAgent->RegisterMethodHandler(TEXT("StopMovement"),       [this]() { return StopMovement();       });
		BehaviacAgent->RegisterMethodHandler(TEXT("LookAround"),         [this]() { return LookAround();         });

		// ── 3. Seed blackboard properties ─────────────────────────────
		BehaviacAgent->SetBoolProperty  (TEXT("IsMoving"),     false);
		BehaviacAgent->SetFloatProperty (TEXT("WanderRadius"), WanderRadius);
		BehaviacAgent->SetFloatProperty (TEXT("WanderSpeed"),  WanderSpeed);
	}

	// ── 4. Load behavior tree (ABehaviacAnimalBase::BeginPlay handles this)
	//      Set BehaviorTree asset reference in BP_Penguin's Details panel, or
	//      set BehaviorTreeAssetPath to "PenguinWanderTree" for path-based load.

	// ── Load behavior tree ─────────────────────────────────────────────
	bool bLoaded = false;
	if (BehaviacAgent)
	{
		if (BehaviorTree)
		{
			bLoaded = BehaviacAgent->LoadBehaviorTree(BehaviorTree);
			UE_LOG(LogTemp, Warning, TEXT("[Behaviac] %s: behavior tree loaded from asset reference"), *GetName());
		}
		else if (!BehaviorTreeAssetPath.IsEmpty())
		{
			BehaviacAgent->LoadBehaviorTreeByPath(BehaviorTreeAssetPath);
			UE_LOG(LogTemp, Warning, TEXT("[Behaviac] %s: behavior tree loaded from asset path"), *GetName());
			bLoaded = true; // path-based load doesn't return bool
		}
	}
	if (bLoaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Behaviac] %s %p: behavior tree loaded OK"), *GetName(), this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[Behaviac] %s %p: failed to load behavior tree!"), *GetName(), this);
		// Early return to avoid null dereference if tree is missing
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[BehaviacPenguin] %s %p: initialized at %s, WanderRadius=%.0f"),
		*GetName(), this, *SpawnLocation.ToString(), WanderRadius);
}

// ============================================================
// Tick
// ============================================================

void ABehaviacPenguin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!BehaviacAgent) return;

	// Sync IsMoving for the animation blueprint (idle → running transition)
	UpdateBehaviacProperties();

	// Manually tick the behavior tree
	TickCounter++;
	EBehaviacStatus Status = BehaviacAgent->TickBehaviorTree();

	// Log every ~5 s at 60 fps
	if (TickCounter % 300 == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[BehaviacPenguin] %s tick#%d → %s | Pos=%s | IsMoving=%s"),
			*GetName(), TickCounter,
			Status == EBehaviacStatus::Running ? TEXT("Running") :
			Status == EBehaviacStatus::Success ? TEXT("Success") : TEXT("Failure/Other"),
			*GetActorLocation().ToString(),
			GetVelocity().SizeSquared2D() > 25.f ? TEXT("true") : TEXT("false"));
	}
}

// ============================================================
// Private helpers
// ============================================================

void ABehaviacPenguin::UpdateBehaviacProperties()
{
	// 5 units/s² threshold avoids flicker while standing still
	bool bMoving = GetVelocity().SizeSquared2D() > 25.f;
	BehaviacAgent->SetBoolProperty(TEXT("IsMoving"), bMoving);
}

// ============================================================
// Behavior-tree actions
// ============================================================

// ──────────────────────────────────────────────────────────────
// PickWanderTarget
//   Selects a random destination within WanderRadius of the spawn
//   location.  Always returns Success.
// ──────────────────────────────────────────────────────────────
EBehaviacStatus ABehaviacPenguin::PickWanderTarget()
{
	// Random angle and distance — spread from 30 % to 100 % of the radius
	// so the penguin never picks a point too close to where it already is.
	float Angle    = FMath::FRandRange(0.f, 2.f * PI);
	float Distance = FMath::FRandRange(WanderRadius * 0.3f, WanderRadius);

	WanderTarget = SpawnLocation + FVector(
		FMath::Cos(Angle) * Distance,
		FMath::Sin(Angle) * Distance,
		0.f);

	bHasWanderTarget = true;

	UE_LOG(LogTemp, Log, TEXT("[BehaviacPenguin] %s: new wander target %s (dist=%.0f)"),
		*GetName(), *WanderTarget.ToString(), Distance);

	return EBehaviacStatus::Success;
}

// ──────────────────────────────────────────────────────────────
// MoveToWanderTarget
//   Issues a navigation move request on the first call, then
//   checks distance each tick.  Returns:
//     Success  — arrived within WanderAcceptanceRadius
//     Running  — still navigating
//     Failure  — no controller / no target / nav failed
// ──────────────────────────────────────────────────────────────
EBehaviacStatus ABehaviacPenguin::MoveToWanderTarget()
{
	if (!bHasWanderTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BehaviacPenguin] MoveToWanderTarget: no target set"));
		return EBehaviacStatus::Failure;
	}

	AAIController* AIC = GetController<AAIController>();
	if (!AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BehaviacPenguin] MoveToWanderTarget: no AIController"));
		return EBehaviacStatus::Failure;
	}

	// Distance check first — avoids issuing unnecessary move requests
	float Dist2D = FVector::Dist2D(GetActorLocation(), WanderTarget);
	if (Dist2D <= WanderAcceptanceRadius)
	{
		AIC->StopMovement();
		return EBehaviacStatus::Success;
	}

	// Set walk speed and request navigation move (matches TestMinion pattern).
	// UE5 path following gracefully handles repeated calls to the same target.
	GetCharacterMovement()->MaxWalkSpeed = WanderSpeed;

	EPathFollowingRequestResult::Type Result = AIC->MoveToLocation(
		WanderTarget,
		WanderAcceptanceRadius * 0.8f,
		/* bStopOnOverlap         */ true,
		/* bUsePathfinding        */ true,
		/* bProjectDestToNav      */ true,
		/* bCanStrafe             */ false);

	if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		return EBehaviacStatus::Success;
	}
	if (Result == EPathFollowingRequestResult::RequestSuccessful)
	{
		return EBehaviacStatus::Running;
	}

	// Pathfinding failed — treat as success to avoid blocking the loop forever
	UE_LOG(LogTemp, Warning, TEXT("[BehaviacPenguin] %s: MoveToLocation failed (target=%s) — skipping"),
		*GetName(), *WanderTarget.ToString());
	return EBehaviacStatus::Success;
}

// ──────────────────────────────────────────────────────────────
// StopMovement
//   Halts navigation.  Always returns Success.
// ──────────────────────────────────────────────────────────────
EBehaviacStatus ABehaviacPenguin::StopMovement()
{
	if (AAIController* AIC = GetController<AAIController>())
	{
		AIC->StopMovement();
	}
	bHasWanderTarget = false;
	return EBehaviacStatus::Success;
}

// ──────────────────────────────────────────────────────────────
// LookAround
//   Snaps the penguin to a random yaw so it appears to look
//   around curiously while resting.  Always returns Success.
//   The Wait node following this in the tree provides the pause.
// ──────────────────────────────────────────────────────────────
EBehaviacStatus ABehaviacPenguin::LookAround()
{
	float RandomYaw = FMath::FRandRange(0.f, 360.f);
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw = RandomYaw;
	SetActorRotation(NewRotation);

	UE_LOG(LogTemp, Log, TEXT("[BehaviacPenguin] %s: looking at yaw=%.1f"),
		*GetName(), RandomYaw);

	return EBehaviacStatus::Success;
}

