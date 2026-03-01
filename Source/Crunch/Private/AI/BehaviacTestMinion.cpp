// Behaviac AI Test Implementation for Minion

#include "AI/BehaviacTestMinion.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "BrainComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/CGameplayAbilityTypes.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviacAgent.h"
#include "BehaviorTree/BehaviacBehaviorTree.h"
#include "BehaviacTypes.h"

ABehaviacTestMinion::ABehaviacTestMinion()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Behaviac agent component
	BehaviacAgent = CreateDefaultSubobject<UBehaviacAgentComponent>(TEXT("BehaviacAgent"));
	// Manually tick from our own Tick() for ordering control
	BehaviacAgent->bAutoTick = false;

	// Defaults
	DetectionRadius     = 1000.0f;
	WalkSpeed           = 200.0f;
	RunSpeed            = 400.0f;
	GuardRadius         = 1500.0f;
	AttackRange         = 200.0f;
	CombatRange         = 250.0f;
	MoveAcceptanceRadius = 100.0f;

	bHasLastKnownPos    = false;
	LookAroundDir       = 1.0f;
	CurrentPatrolIndex  = 0;
	TickCounter         = 0;
	DebugTimer          = 0.0f;
	PropertyUpdateInterval   = 0.2f;
	LastPropertyUpdateTime   = 0.0f;
}

void ABehaviacTestMinion::BeginPlay()
{

	Super::BeginPlay();

	if (bUseBehaviacAI)
	{
		// Disable native UE5 BT system if running
		if (AAIController* AIC = GetController<AAIController>())
		{
			if (AIC->BrainComponent)
			{
				AIC->BrainComponent->StopLogic("Behaviac AI enabled");
				BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s: stopped native UE5 BT system"), *GetName());
			}
		}
	}

	if (!bUseBehaviacAI || !BehaviacAgent)
	{
		BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s: using default UE5 BT"), *GetName());
		return;
	}

	BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s: initializing Behaviac AI"), *GetName());

	// Guard post at spawn
	GuardCenter = GetActorLocation();

	// Build patrol ring around spawn
	FVector Origin = GetActorLocation();
	PatrolPoints.Add(Origin + FVector( 500,    0, 0));
	PatrolPoints.Add(Origin + FVector( 500,  500, 0));
	PatrolPoints.Add(Origin + FVector(   0,  500, 0));
	PatrolPoints.Add(Origin);

	// ── Register all method handlers (lambda style, matches BehaviacAINPC) ──

	BehaviacAgent->RegisterMethodHandler(TEXT("FindPlayer"),        [this]() { return FindPlayer(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("HasTarget"),         [this]() { return FindPlayer(); });   // alias
	BehaviacAgent->RegisterMethodHandler(TEXT("InAttackRange"),     [this]() -> EBehaviacStatus {
		if (!CurrentTarget) return EBehaviacStatus::Failure;
		float Dist = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());
		return (Dist <= AttackRange) ? EBehaviacStatus::Success : EBehaviacStatus::Failure;
	});
	BehaviacAgent->RegisterMethodHandler(TEXT("MoveToTarget"),      [this]() { return MoveToTarget(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("Patrol"),            [this]() { return Patrol(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("PatrolToGoal"),      [this]() { return PatrolToGoal(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("UpdateAIState"),     [this]() { return UpdateAIState(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("ChasePlayer"),       [this]() { return ChasePlayer(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("AttackTarget"),      [this]() { return AttackTarget(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("StopMovement"),      [this]() { return StopMovement(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("FaceTarget"),        [this]() { return FaceTarget(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("SetWalkSpeed"),      [this]() { return SetWalkSpeed(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("SetRunSpeed"),       [this]() { return SetRunSpeed(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("MoveToLastKnownPos"),[this]() { return MoveToLastKnownPos(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("LookAround"),        [this]() { return LookAround(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("ClearLastKnownPos"), [this]() { return ClearLastKnownPos(); });
	BehaviacAgent->RegisterMethodHandler(TEXT("ReturnToPost"),      [this]() { return ReturnToPost(); });

	// ── Seed initial blackboard properties ─────────────────────────────
	BehaviacAgent->SetIntProperty  (TEXT("Health"),           100);
	BehaviacAgent->SetBoolProperty (TEXT("HasTarget"),        false);
	BehaviacAgent->SetFloatProperty(TEXT("DistanceToTarget"), 999999.0f);
	BehaviacAgent->SetBoolProperty (TEXT("IsMoving"),         false);
	BehaviacAgent->SetFloatProperty(TEXT("DetectionRadius"),  DetectionRadius);
	BehaviacAgent->SetFloatProperty(TEXT("WalkSpeed"),        WalkSpeed);
	BehaviacAgent->SetFloatProperty(TEXT("RunSpeed"),         RunSpeed);
	BehaviacAgent->SetPropertyValue(TEXT("AIState"),          TEXT("Patrol"));

	// ── Load behavior tree ─────────────────────────────────────────────
	bool bLoaded = false;
	if (BehaviorTree)
	{
		bLoaded = BehaviacAgent->LoadBehaviorTree(BehaviorTree);
	}
	else if (!BehaviorTreeAssetPath.IsEmpty())
	{
		BehaviacAgent->LoadBehaviorTreeByPath(BehaviorTreeAssetPath);
		bLoaded = true; // path-based load doesn't return bool
	}

	if (bLoaded)
	{
		BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s: behavior tree loaded OK"), *GetName());
	}
	else
	{
		UE_LOG(LogBehaviac, Error, TEXT("[BehaviacTestMinion] %s: failed to load behavior tree!"), *GetName());
	}
}

void ABehaviacTestMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bUseBehaviacAI || !BehaviacAgent)
	{
		return;
	}

	// Periodic blackboard sync
	LastPropertyUpdateTime += DeltaTime;
	if (LastPropertyUpdateTime >= PropertyUpdateInterval)
	{
		UpdateBehaviacProperties();
		LastPropertyUpdateTime = 0.0f;
	}

	// Tick behavior tree
	TickCounter++;
	EBehaviacStatus Status = BehaviacAgent->TickBehaviorTree();

	// Debug every ~2s at 60fps
	if (TickCounter % 120 == 0)
	{
		BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s tick #%d → %s | AIState=%s"),
			*GetName(), TickCounter,
			Status == EBehaviacStatus::Running ? TEXT("Running") :
			Status == EBehaviacStatus::Success ? TEXT("Success") : TEXT("Failure"),
			*BehaviacAgent->GetPropertyValue(TEXT("AIState")));
	}

	// 5-second movement debug
	DebugTimer += DeltaTime;
	if (DebugTimer >= 5.0f)
	{
		BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s Pos=%s Vel=%.0f"),
			*GetName(), *GetActorLocation().ToString(), GetVelocity().Size());
		DebugTimer = 0.0f;
	}
}

// ============================================================
// Goal assignment (called by MinionBarrack)
// ============================================================

void ABehaviacTestMinion::SetGoal(AActor* Goal)
{
	// When using Behaviac AI the native UBlackboardComponent is not initialised
	// (BT was stopped in BeginPlay), so bypass AMinion::SetGoal and store the
	// goal actor directly for PatrolToGoal() to use.
	if (bUseBehaviacAI)
	{
		GoalActor = Goal;
		BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s: GoalActor set to %s"),
			*GetName(), Goal ? *Goal->GetName() : TEXT("null"));
		return;
	}

	// Behaviac disabled — delegate to native blackboard path.
	Super::SetGoal(Goal);
}

// ============================================================
// Target detection
// ============================================================

bool ABehaviacTestMinion::FindTargetViaPerception()
{
	// Use AIPerception's hostile-only list first — mirrors ACAIController which
	// configures SightConfig with bDetectEnemies=true / bDetectFriendlies=false.
	if (AAIController* AIC = GetController<AAIController>())
	{
		if (UAIPerceptionComponent* Perc = AIC->GetAIPerceptionComponent())
		{
			TArray<AActor*> HostileActors;
			Perc->GetPerceivedHostileActors(HostileActors);
			if (HostileActors.Num() > 0)
			{
				CurrentTarget = HostileActors[0];
				return true;
			}
		}
	}

	// Fallback: manual radius scan, but only accept actors that are
	// ETeamAttitude::Hostile toward us (same logic as the rest of the codebase).
	if (APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) <= DetectionRadius)
		{
			if (GetTeamAttitudeTowards(*Player) == ETeamAttitude::Hostile)
			{
				CurrentTarget = Player;
				return true;
			}
		}
	}

	CurrentTarget = nullptr;
	return false;
}

// ============================================================
// Behaviac Actions
// ============================================================

EBehaviacStatus ABehaviacTestMinion::FindPlayer()
{
	bool bFound = FindTargetViaPerception();
	if (BehaviacAgent)
	{
		BehaviacAgent->SetBoolProperty(TEXT("HasTarget"), bFound);
	}
	return bFound ? EBehaviacStatus::Success : EBehaviacStatus::Failure;
}

EBehaviacStatus ABehaviacTestMinion::MoveToTarget()
{
	if (!CurrentTarget)
	{
		BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] MoveToTarget: no target"));
		return EBehaviacStatus::Failure;
	}

	AAIController* AIC = GetController<AAIController>();
	if (!AIC) return EBehaviacStatus::Failure;

	float Dist = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());

	// Already in attack range — report Success so the Sequence advances to FaceTarget/Attack.
	if (Dist <= AttackRange)
	{
		AIC->StopMovement();
		return EBehaviacStatus::Success;
	}

	GetCharacterMovement()->MaxWalkSpeed = (Dist > DetectionRadius * 0.5f) ? RunSpeed : WalkSpeed;

	EPathFollowingRequestResult::Type Result = AIC->MoveToActor(
		CurrentTarget, AttackRange * 0.8f, true, true, false, nullptr, true);

	// AlreadyAtGoal also means we're close enough — treat as success.
	if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		return EBehaviacStatus::Success;
	}
	if (Result == EPathFollowingRequestResult::RequestSuccessful)
	{
		return EBehaviacStatus::Running;
	}

	BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] MoveToTarget: move request failed"));
	return EBehaviacStatus::Failure;
}

EBehaviacStatus ABehaviacTestMinion::Patrol()
{
	if (PatrolPoints.Num() == 0) return EBehaviacStatus::Failure;

	AAIController* AIC = GetController<AAIController>();
	if (!AIC) return EBehaviacStatus::Failure;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	FVector Target = PatrolPoints[CurrentPatrolIndex];

	if (FVector::Dist(GetActorLocation(), Target) < 100.0f)
	{
		CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
		Target = PatrolPoints[CurrentPatrolIndex];
		AIC->MoveToLocation(Target, 50.0f);
		return EBehaviacStatus::Running;
	}

	UPathFollowingComponent* PF = AIC->GetPathFollowingComponent();
	if (PF && PF->GetStatus() == EPathFollowingStatus::Moving)
	{
		return EBehaviacStatus::Running;
	}

	EPathFollowingRequestResult::Type Result = AIC->MoveToLocation(Target, 50.0f);
	if (Result == EPathFollowingRequestResult::Failed)
	{
		BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] Patrol: MoveToLocation failed (no NavMesh?)"));
		return EBehaviacStatus::Failure;
	}
	return EBehaviacStatus::Running;
}

EBehaviacStatus ABehaviacTestMinion::PatrolToGoal()
{
	AAIController* AIC = GetController<AAIController>();
	if (!AIC) return EBehaviacStatus::Failure;

	// ── If a Goal actor is assigned (set by MinionBarrack), march toward it ──
	if (GoalActor)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		// Already at goal — keep returning Success so the BT loops back here
		// and the minion idles at the objective.
		if (FVector::Dist(GetActorLocation(), GoalActor->GetActorLocation()) < MoveAcceptanceRadius)
		{
			return EBehaviacStatus::Success;
		}

		UPathFollowingComponent* PF = AIC->GetPathFollowingComponent();
		if (PF && PF->GetStatus() == EPathFollowingStatus::Moving)
		{
			return EBehaviacStatus::Running;
		}

		EPathFollowingRequestResult::Type Result = AIC->MoveToActor(GoalActor, MoveAcceptanceRadius);
		if (Result == EPathFollowingRequestResult::Failed)
		{
			BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s PatrolToGoal: MoveToActor failed (no NavMesh?)"), *GetName());
			return EBehaviacStatus::Failure;
		}
		return EBehaviacStatus::Running;
	}

	// ── No goal assigned: fall back to waypoint patrol ring ──────────────────
	return Patrol();
}

EBehaviacStatus ABehaviacTestMinion::UpdateAIState()
{
	FString NewState = TEXT("Patrol");
	float DistFromPost = FVector::Dist(GetActorLocation(), GuardCenter);

	AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Player)
	{
		// Also check perception
		FindTargetViaPerception();
		Player = CurrentTarget;
	}

	if (Player)
	{
		float DistToPlayer    = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
		float PlayerFromPost  = FVector::Dist(Player->GetActorLocation(), GuardCenter);

		// Line-of-sight check
		bool bCanSee = false;
		if (DistToPlayer <= DetectionRadius)
		{
			FVector EyeLoc     = GetActorLocation() + FVector(0, 0, 60.f);
			FVector PlayerEye  = Player->GetActorLocation() + FVector(0, 0, 60.f);
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			Params.AddIgnoredActor(Player);
			bool bBlocked = GetWorld()->LineTraceSingleByChannel(Hit, EyeLoc, PlayerEye, ECC_Visibility, Params);
			bCanSee = !bBlocked;
		}

		if (bCanSee && DistToPlayer <= AttackRange)
		{
			CurrentTarget = Player;
			bHasLastKnownPos = true;
			LastKnownPlayerPos = Player->GetActorLocation();
			NewState = TEXT("Combat");
		}
		else if (bCanSee && PlayerFromPost <= GuardRadius)
		{
			CurrentTarget = Player;
			bHasLastKnownPos = true;
			LastKnownPlayerPos = Player->GetActorLocation();
			NewState = TEXT("Chase");
		}
		else if (bCanSee && PlayerFromPost > GuardRadius)
		{
			CurrentTarget = nullptr;
			bHasLastKnownPos = false;
			LastKnownPlayerPos = FVector::ZeroVector;
			NewState = TEXT("ReturnToPost");
		}
		else
		{
			// Cannot see player
			if (bHasLastKnownPos || CurrentTarget != nullptr)
			{
				CurrentTarget = nullptr;
				bHasLastKnownPos = false;
				LastKnownPlayerPos = FVector::ZeroVector;
				NewState = TEXT("ReturnToPost");
			}
			else
			{
				NewState = (DistFromPost > GuardRadius) ? TEXT("ReturnToPost") : TEXT("Patrol");
			}
		}
	}
	else
	{
		CurrentTarget = nullptr;
		bHasLastKnownPos = false;
		NewState = (DistFromPost > GuardRadius) ? TEXT("ReturnToPost") : TEXT("Patrol");
	}

	if (BehaviacAgent)
	{
		FString OldState = BehaviacAgent->GetPropertyValue(TEXT("AIState"));
		if (OldState != NewState)
		{
			BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s AIState: %s → %s"), *GetName(), *OldState, *NewState);
			BehaviacAgent->SetPropertyValue(TEXT("AIState"), NewState);
		}
	}

	return EBehaviacStatus::Success;
}

EBehaviacStatus ABehaviacTestMinion::ChasePlayer()
{
	if (BehaviacAgent && BehaviacAgent->GetPropertyValue(TEXT("AIState")) != TEXT("Chase"))
	{
		if (AAIController* AIC = GetController<AAIController>()) AIC->StopMovement();
		return EBehaviacStatus::Failure;
	}
	if (!CurrentTarget) return EBehaviacStatus::Failure;

	AAIController* AIC = GetController<AAIController>();
	if (!AIC) return EBehaviacStatus::Failure;

	float Dist = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());
	if (Dist <= AttackRange) return EBehaviacStatus::Success;

	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	AIC->MoveToActor(CurrentTarget, AttackRange * 0.8f);
	return EBehaviacStatus::Running;
}

EBehaviacStatus ABehaviacTestMinion::AttackTarget()
{
	if (!CurrentTarget) return EBehaviacStatus::Failure;

	// Check still in range — if target moved away, fail so the Sequence restarts
	// from MoveToTarget and closes the gap again.
	float Dist = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());
	if (Dist > CombatRange)
	{
		return EBehaviacStatus::Failure;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);
	if (!ASC)
	{
		UE_LOG(LogBehaviac, Error, TEXT("[BehaviacTestMinion] %s: no AbilitySystemComponent!"), *GetName());
		return EBehaviacStatus::Failure;
	}

	// GA_Combo is driven by WaitInputPress internally — it plays the montage and
	// waits for repeated presses to chain sections.  Mirror what the native BT's
	// SendInputToAbilitySystem does: press the input to activate/advance the combo.
	// Return Running so the BT stays here (the XML Wait node handles timing between
	// presses to match the combo window).
	ASC->PressInputID(static_cast<int32>(ECAbilityInputID::BasicAttack));
	BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s: PressInputID BasicAttack"), *GetName());
	return EBehaviacStatus::Running;
}

EBehaviacStatus ABehaviacTestMinion::StopMovement()
{
	if (AAIController* AIC = GetController<AAIController>()) AIC->StopMovement();
	return EBehaviacStatus::Success;
}

EBehaviacStatus ABehaviacTestMinion::FaceTarget()
{
	if (!CurrentTarget) return EBehaviacStatus::Failure;
	FVector Dir = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator Look = Dir.Rotation();
	Look.Pitch = 0.f;
	Look.Roll  = 0.f;
	SetActorRotation(Look);
	return EBehaviacStatus::Success;
}

EBehaviacStatus ABehaviacTestMinion::SetWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	return EBehaviacStatus::Success;
}

EBehaviacStatus ABehaviacTestMinion::SetRunSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	return EBehaviacStatus::Success;
}

EBehaviacStatus ABehaviacTestMinion::MoveToLastKnownPos()
{
	if (!bHasLastKnownPos) return EBehaviacStatus::Failure;

	AAIController* AIC = GetController<AAIController>();
	if (!AIC) return EBehaviacStatus::Failure;

	if (FVector::Dist(GetActorLocation(), LastKnownPlayerPos) < 100.0f)
	{
		return EBehaviacStatus::Success;
	}

	EPathFollowingRequestResult::Type Result = AIC->MoveToLocation(LastKnownPlayerPos, 80.0f);
	return (Result != EPathFollowingRequestResult::Failed)
		? EBehaviacStatus::Running : EBehaviacStatus::Failure;
}

EBehaviacStatus ABehaviacTestMinion::LookAround()
{
	FRotator Current = GetActorRotation();
	Current.Yaw += 45.0f * LookAroundDir;
	LookAroundDir = -LookAroundDir;
	SetActorRotation(Current);
	return EBehaviacStatus::Success;
}

EBehaviacStatus ABehaviacTestMinion::ClearLastKnownPos()
{
	bHasLastKnownPos = false;
	LastKnownPlayerPos = FVector::ZeroVector;
	CurrentTarget = nullptr;
	if (BehaviacAgent)
	{
		BehaviacAgent->SetPropertyValue(TEXT("AIState"), TEXT("Patrol"));
	}
	BEHAVIAC_VLOG(TEXT("[BehaviacTestMinion] %s: investigation complete, back to patrol"), *GetName());
	return EBehaviacStatus::Success;
}

EBehaviacStatus ABehaviacTestMinion::ReturnToPost()
{
	AAIController* AIC = GetController<AAIController>();
	if (!AIC) return EBehaviacStatus::Failure;

	if (FVector::Dist(GetActorLocation(), GuardCenter) < 100.0f)
	{
		if (BehaviacAgent)
		{
			BehaviacAgent->SetPropertyValue(TEXT("AIState"), TEXT("Patrol"));
		}
		return EBehaviacStatus::Success;
	}

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	EPathFollowingRequestResult::Type Result = AIC->MoveToLocation(GuardCenter, 80.0f);
	return (Result != EPathFollowingRequestResult::Failed)
		? EBehaviacStatus::Running : EBehaviacStatus::Failure;
}

bool ABehaviacTestMinion::IsPlayerInRange()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Player) return false;
	return FVector::Dist(GetActorLocation(), Player->GetActorLocation()) <= DetectionRadius;
}

// ============================================================
// Blackboard property sync
// ============================================================

void ABehaviacTestMinion::UpdateBehaviacProperties()
{
	if (!BehaviacAgent) return;

	// Health via GAS (placeholder — extend when GAS attribute getter is available)
	BehaviacAgent->SetIntProperty(TEXT("Health"), 100);

	BehaviacAgent->SetBoolProperty(TEXT("HasTarget"), CurrentTarget != nullptr);

	float Dist = 999999.0f;
	if (CurrentTarget)
	{
		Dist = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());
	}
	BehaviacAgent->SetFloatProperty(TEXT("DistanceToTarget"), Dist);

	bool bMoving = GetCharacterMovement() && GetCharacterMovement()->Velocity.SizeSquared() > 100.0f;
	BehaviacAgent->SetBoolProperty(TEXT("IsMoving"), bMoving);
}
