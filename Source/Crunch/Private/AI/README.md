# AI Subsystem Documentation

Updated: 2025-08-13

This document provides a class-by-class interpretation of the AI subsystem under `Source/Crunch/Private/AI`. It summarizes interfaces, methods, properties, and how they integrate with GAS, Perception, Blackboard, Teams, and Spawning.

Contents
- UBTTask_SendInputToAbilitySystem (Behavior Tree Task)
- ACAIController (AI Controller)
- AMinion (AI Pawn)
- AMinionBarrack (Spawner/Pool)
- Cross-cutting Integrations
- Typical Behavior Flow
- Extension Points & Notes

## UBTTask_SendInputToAbilitySystem
Files: `BTTask_SendInputToAbilitySystem.h/.cpp`

- Class: `UBTTask_SendInputToAbilitySystem : UBTTaskNode`
- Includes: `BehaviorTree/BTTaskNode.h`, `GAS/CGameplayAbilityTypes.h`
- Properties
  - `UPROPERTY(EditAnywhere, Category = "Ability") ECAbilityInputID InputID` — Ability input to press.
- Methods
  - `virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override`
    - Steps:
      1. `AAIController*` via `OwnerComp.GetAIOwner()`.
      2. Pawn ASC via `UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AI->GetPawn())`.
      3. If ASC: `ASC->PressInputID((int32)InputID)` → `Succeeded`; otherwise `Failed`.
- Purpose
  - Bridge between Behavior Trees and GAS to trigger ability inputs programmatically.

## ACAIController
Files: `CAIController.h/.cpp`

- Class: `ACAIController : AAIController`
- Properties
  - `UPROPERTY(EditDefaultsOnly, Category = "AI Behavior") FName TargetBlackboardKeyName = "Target"` — Blackboard key for current target.
  - `UPROPERTY(EditDefaultsOnly, Category = "AI Behavior") UBehaviorTree* BehaviorTree`
  - `UPROPERTY(VisibleDefaultsOnly, Category = "Perception") UAIPerceptionComponent* AIPerceptionComponent`
  - `UPROPERTY(VisibleDefaultsOnly, Category = "Perception") UAISenseConfig_Sight* SightConfig`
  - `bool bIsPawnDead = false`
- Constructor
  - Creates `AIPerceptionComponent` and `SightConfig`.
  - Sight settings: Enemies-only; `SightRadius = 1000`, `LoseSightRadius = 1200`, `MaxAge = 5`, `PeripheralVisionAngleDegrees = 180`.
  - Configures sense and binds delegates:
    - `OnTargetPerceptionUpdated` → `TargetPerceptionUpdated`
    - `OnTargetPerceptionForgotten` → `TargetForgotten`
- Methods
  - `virtual void OnPossess(APawn* NewPawn) override`
    - If pawn implements `IGenericTeamAgentInterface`: copy `GenericTeamId`, then `ClearAndDisableAllSenses()` and `EnableAllSenses()`.
    - Subscribes pawn ASC tag events:
      - Dead tag → `PawnDeadTagUpdated`
      - Stun tag → `PawnStunTagUpdated`
  - `virtual void BeginPlay() override`
    - `RunBehaviorTree(BehaviorTree)`
  - `void TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)`
    - If sensed and no current target → `SetCurrentTarget(TargetActor)`; else `ForgetActorIfDead(TargetActor)`.
  - `void TargetForgotten(AActor* ForgottenActor)`
    - If forgotten equals current target → `SetCurrentTarget(GetNextPerceivedActor())`.
  - `const UObject* GetCurrentTarget() const` — Reads blackboard value at `TargetBlackboardKeyName`.
  - `void SetCurrentTarget(AActor* NewTarget)` — Sets/clears blackboard key.
  - `AActor* GetNextPerceivedActor() const` — Returns first actor from `GetPerceivedHostileActors` or `nullptr`.
  - `void ForgetActorIfDead(AActor* ActorToForget)` — If actor ASC has Dead tag, ages stimuli to max so perception forgets it.
  - `void ClearAndDisableAllSenses()` — Ages all stimuli; disables senses; clears target.
  - `void EnableAllSenses()` — Re-enables senses.
  - `void PawnDeadTagUpdated(const FGameplayTag Tag, int32 Count)` — Stop/Start brain and toggle senses; update `bIsPawnDead`.
  - `void PawnStunTagUpdated(const FGameplayTag Tag, int32 Count)` — If dead ignore; otherwise Stop/Start brain for stun.
- Purpose
  - Target management via perception + blackboard; reacts to GAS Dead/Stun states; team-affiliation consistent.

## AMinion
Files: `Minion.h/.cpp`

- Class: `AMinion : ACCharacter`
- Properties
  - `UPROPERTY(EditDefaultsOnly, Category = "AI") FName GoalBlackboardKeyName = "Goal"` — Blackboard key for goal.
  - `UPROPERTY(EditDefaultsOnly, Category = "Visual") TMap<FGenericTeamId, USkeletalMesh*> SkinMap` — Team → mesh.
- Methods
  - `virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override` — Super + `PickSkinBasedOnTeamID()`.
  - `bool IsActive() const` — `!IsDead()`.
  - `void Activate()` — `RespawnImmediately()`.
  - `void SetGoal(AActor* Goal)` — Writes `Goal` to controller’s blackboard if available.
  - `void PickSkinBasedOnTeamID()` — Finds mesh by `GetGenericTeamId()` and applies via `GetMesh()->SetSkeletalMesh()`.
  - `virtual void OnRep_TeamID() override` — Re-apply skin on team replication.
- Purpose
  - AI pawn with team-based appearance and blackboard-configurable goal.

## AMinionBarrack
Files: `MinionBarrack.h/.cpp`

- Class: `AMinionBarrack : AActor`
- Properties
  - `UPROPERTY(EditAnywhere, Category = "Spawn") FGenericTeamId BarrackTeamId`
  - `UPROPERTY(EditAnywhere, Category = "Spawn") int MinionPerGroup = 3`
  - `UPROPERTY(EditAnywhere, Category = "Spawn") float GroupSpawnInterval = 5.f`
  - `UPROPERTY() TArray<class AMinion*> MinionPool`
  - `UPROPERTY(EditAnywhere, Category = "Spawn") AActor* Goal`
  - `UPROPERTY(EditAnywhere, Category = "Spawn") TSubclassOf<class AMinion> MinionClass`
  - `UPROPERTY(EditAnywhere, Category = "Spawn") TArray<class APlayerStart*> SpawnSpots`
  - `int NextSpawnSpotIndex = -1`
  - `FTimerHandle SpawnIntervalTimerHandle`
- Lifecycle
  - Constructor: `PrimaryActorTick.bCanEverTick = true`.
  - `BeginPlay()`
    - If `HasAuthority()`: set repeating timer to `SpawnNewGroup` every `GroupSpawnInterval` seconds.
  - `Tick(float DeltaTime)`: default tick.
- Methods
  - `const APlayerStart* GetNextSpawnSpot()` — Round-robin through `SpawnSpots`; returns `nullptr` if empty.
  - `void SpawnNewGroup()`
    - Tries to activate `MinionPerGroup` minions from pool; if not enough, spawns remainder:
      - Choose transform (next spawn spot if any, else self).
      - `AMinion*` from `GetNextAvaliableMinion()`; if found, set transform and `Activate()`.
      - After loop, call `SpawnNewMinions(i)` for remaining.
  - `void SpawnNewMinions(int Amt)`
    - For each:
      - Choose transform as above.
      - `SpawnActorDeferred<AMinion>(MinionClass, ..., ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)`.
      - `SetGenericTeamId(BarrackTeamId)`, `FinishSpawning`, `SetGoal(Goal)`, add to `MinionPool`.
  - `AMinion* GetNextAvaliableMinion() const` — First `!IsActive()` minion in pool; else `nullptr`.
- Purpose
  - Server-side spawner/pool for periodic minion groups, with team assignment, goal setup, and spawn spots.

## Cross-cutting Integrations
- Perception & Blackboard
  - `ACAIController` maintains a `Target` blackboard value based on sighted hostile actors.
  - `AMinion` writes a `Goal` blackboard value for its own behavior logic.
- GAS (Gameplay Ability System)
  - `ACAIController` subscribes to pawn Dead/Stun gameplay tags to pause/resume brain and senses.
  - `UBTTask_SendInputToAbilitySystem` triggers `PressInputID` on ASC from BT tasks.
- Teams
  - `AMinion` and `ACAIController` use `IGenericTeamAgentInterface` for consistent team behavior and visuals.
- Spawning & Pooling
  - `AMinionBarrack` reuses minions via a simple pool and only spawns new ones when needed.

## Typical Behavior Flow
1. `AMinionBarrack` periodically calls `SpawnNewGroup()` (server-only). It activates pooled minions or spawns new ones, assigns team and goal.
2. Each `AMinion` runs under `ACAIController`, which starts its `BehaviorTree` on `BeginPlay`.
3. `ACAIController` perceives hostiles via sight and writes the first hostile into the `Target` blackboard key.
4. Behavior Tree logic can use `UBTTask_SendInputToAbilitySystem` to trigger GAS abilities using configured `ECAbilityInputID`.
5. If a pawn gains the Dead tag, `ACAIController` stops logic and disables senses; if Stun, it pauses logic until the tag is removed.

## Extension Points & Notes
- Add additional senses (hearing, damage) by creating and configuring corresponding `UAISenseConfig_*` and calling `ConfigureSense`.
- Expand behavior by adding more BT tasks or services (e.g., pathing to `Goal`, selecting threats, cover, etc.).
- Team visuals for `AMinion` are data-driven via `SkinMap`; ensure all teams you use have a mapped mesh.
- Consider exposing more `ACAIController` parameters (radii, FOV, tag keys) to data assets for designers.
- Pool sizing: `AMinionBarrack` pools on demand; you can pre-warm the pool at `BeginPlay` if spikes are expected.
