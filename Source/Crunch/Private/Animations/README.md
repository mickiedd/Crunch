# Animations Subsystem Documentation

Updated: 2025-08-13

This document provides a class-by-class interpretation of the Animations subsystem under `Source/Crunch/Private/Animations`. It summarizes interfaces, methods, properties, and how they integrate with GAS, Character Movement, and Gameplay Cues.

Contents
- UCAnimInstance (Animation Instance)
- UAN_SendGameplayEvent (Anim Notify)
- UAN_SendTargetGroup (Anim Notify)
- Cross-cutting Integrations
- Typical Animation Flow
- Extension Points & Notes

## UCAnimInstance
Files: `CAnimInstance.h/.cpp`

- Class: `UCAnimInstance : UAnimInstance`
- Includes: `Animation/AnimInstance.h`, `GameplayTagContainer.h`
- Responsibilities
  - Cache owner `ACharacter` and `UCharacterMovementComponent`.
  - Compute locomotion data: speed magnitude, planar forward/right speeds.
  - Compute yaw speed and smoothed yaw speed using configurable lerp rates.
  - Track jumping/ground state and aiming state (via gameplay tag changes).
  - Compute look rotation offsets (Yaw/Pitch) relative to body rotation.
- Key Methods
  - `NativeInitializeAnimation()`: Cache references to owner and movement.
  - `NativeUpdateAnimation(float)`: Gather/compute per-frame values.
  - `NativeThreadSafeUpdateAnimation(float)`: Thread-safe updates for linked instances.
  - `OwnerAimTagChanged(FGameplayTag, int32)`: Update `bIsAimming` on tag changes.
  - `ShouldDoFullBody() const`: Helper for anim graph branching (full-body vs layered).
- Properties
  - Cached: `OwnerCharacter`, `OwnerMovementComp`.
  - Runtime: `Speed`, `FwdSpeed`, `RightSpeed`, `YawSpeed`, `SmoothedYawSpeed`, `bIsJumping`, `bIsAimming`, `BodyPrevRot`, `LookRotOffset`.
  - Tuning: `YawSpeedSmoothLerpSpeed`, `YawSpeedLerpToZeroSpeed`.
- Notes
  - All public getters are `BlueprintCallable` and `BlueprintThreadSafe` so they can be used inside anim graphs safely.

## UAN_SendGameplayEvent
Files: `AN_SendGameplayEvent.h/.cpp`

- Class: `UAN_SendGameplayEvent : UAnimNotify`
- Includes: `Animation/AnimNotifies/AnimNotify.h`, `GameplayTagContainer.h`
- Purpose
  - At the notify frame, send a GAS gameplay event using a configurable `EventTag`.
  - Enables frame-accurate ability event timing (damage windows, VFX/SFX triggers).
- Key Methods
  - `Notify(USkeletalMeshComponent*, UAnimSequenceBase*, const FAnimNotifyEventReference&)`: Dispatch the gameplay event to the appropriate receiver/ASC.
  - `GetNotifyName_Implementation() const`: Editor-friendly label.
- Properties
  - `EventTag`: `FGameplayTag` identifying the event to send.

## UAN_SendTargetGroup
Files: `AN_SendTargetGroup.h/.cpp`

- Class: `UAN_SendTargetGroup : UAnimNotify`
- Includes: `Animation/AnimNotifies/AnimNotify.h`, `GameplayTagContainer.h`, `GenericTeamAgentInterface.h`
- Purpose
  - Perform one or more sphere sweeps around specified mesh sockets to collect nearby actors matching a team attitude, then send local gameplay cues and/or a gameplay event to each.
- Key Methods
  - `Notify(USkeletalMeshComponent*, UAnimSequenceBase*, const FAnimNotifyEventReference&)`: Execute sweeps, filter by team, trigger cues/events per hit via `SendLocalGameplayCue`.
  - `SendLocalGameplayCue(const FHitResult&) const`: Helper to emit cue on hit actor.
- Properties
  - `TriggerGameplayCueTags`: Gameplay cue tags to trigger locally on targets.
  - `TargetTeam`: `ETeamAttitude::Type` filter (Hostile, Friendly, Neutral).
  - `SphereSweepRadius`: Sweep radius around each socket.
  - `bDrawDebug`: Visualize sweeps and hits for debugging.
  - `bIgnoreOwner`: Skip the owner actor in results.
  - `EventTag`: Optional gameplay event tag to send in addition to cues.
  - `TargetSocketNames`: Mesh socket names to use as sweep centers.

## Cross-cutting Integrations
- GAS (Gameplay Ability System)
  - Animation notifies send gameplay events and/or cues to drive abilities and FX.
  - `UCAnimInstance` listens to gameplay tags (Aim) to reflect ability state in animations.
- Character Movement
  - `UCAnimInstance` relies on `UCharacterMovementComponent` for velocity and ground state.
- Teams
  - `UAN_SendTargetGroup` filters sweep results using `IGenericTeamAgentInterface` and `ETeamAttitude`.

## Typical Animation Flow
1. `UCAnimInstance` initializes and caches owner references.
2. Each tick, it updates locomotion metrics, yaw speeds, and look offsets, exposing these to the anim graph.
3. During montage playback, `UAN_SendGameplayEvent` and/or `UAN_SendTargetGroup` fire on specific frames:
   - Send precise GAS gameplay events (e.g., combo stage, damage window start).
   - Perform area/sweep detection around sockets and trigger local cues on hits.
4. Abilities and effects consume these events/cues to apply gameplay logic and FX.

## Extension Points & Notes
- Add more state in `UCAnimInstance` (e.g., crouch, ADS, lean) and expose getters.
- Extend `UAN_SendTargetGroup` to support different shape tests (capsule/box) or line traces.
- Add per-socket radii or per-tag filters for finer control.
- Ensure notifies are placed at correct montage frames to match gameplay timing.
