# Character Subsystem Documentation

Updated: 2025-08-13

This document provides a class-by-class interpretation of the Character subsystem under `Source/Crunch/Private/Character`. It summarizes interfaces, methods, properties, and how they integrate with GAS, Teams, UI, AI Perception, and animation.

Contents
- ACCharacter (Base Character)
- UPA_CharacterDefination (Primary Data Asset)
- Cross-cutting Integrations
- Typical Character Flow
- Extension Points & Notes

## ACCharacter
Files: `CCharacter.h/.cpp`

- Class: `ACCharacter : ACharacter, IAbilitySystemInterface, IGenericTeamAgentInterface, IRenderActorTargetInterface`
- Includes: `GameFramework/Character.h`, `AbilitySystemInterface.h`, `GenericTeamAgentInterface.h`, `GameplayTagContainer.h`, `GameplayEffectTypes.h`, project GAS headers
- Responsibilities
  - GAS integration: exposes `UCAbilitySystemComponent`, handles gameplay tag changes (Dead/Stun/Aim/Focus), attribute changes (move speed, acceleration, max health/mana), and server RPC `Server_SendGameplayEventToSelf`.
  - Lifecycle/state: stun handling (`OnStun`, `OnRecoverFromStun`), death/respawn flow with ragdoll and death montage timing (`StartDeathSequence`, `PlayDeathAnimation`, `DeathMontageFinished`, `RespawnImmediately`, `Respawn`).
  - Teams: implements `IGenericTeamAgentInterface` with replicated `TeamID` and `OnRep_TeamID`.
  - UI: overhead status widget component and distance-based visibility checks via timer.
  - AI: registers and toggles `UAIPerceptionStimuliSourceComponent` for AI sight interaction.
  - Capture interface: `GetCaptureLocalPosition/Rotation` for UI/preview rendering (e.g., headshot frames).
- Key Methods
  - Init/replication: `ServerSideInit`, `ClientSideInit`, `GetLifetimeReplicatedProps`.
  - GAS: `GetAbilitySystemComponent`, `UpgradeAbilityWithInputID`, tag change handlers (`DeathTagUpdated`, `StunTagUpdated`, `AimTagUpdated`, `FocusTagUpdated`), attribute change handlers (`MoveSpeedUpdated`, `MoveSpeedAccelerationUpdated`, `MaxHealthUpdated`, `MaxManaUpdated`).
  - UI: `ConfigureOverHeadStatusWidget`, `UpdateHeadGaugeVisibility`, `SetStatusGaugeEnabled`.
  - Death/respawn: `IsDead`, `StartDeathSequence`, `PlayDeathAnimation`, `SetRagdollEnabled`, `DeathMontageFinished`, `Respawn`, `OnDead`, `OnRespawn`.
  - Teams: `SetGenericTeamId`, `GetGenericTeamId`, `OnRep_TeamID`.
  - AI: `SetAIPerceptionStimuliSourceEnabled`.
- Properties
  - GAS: `UCAbilitySystemComponent* CAbilitySystemComponent`, `UCAttributeSet* CAttributeSet`.
  - UI: `UWidgetComponent* OverHeadWidgetComponent`, timing/range parameters, timer handles.
  - Stun/Death: `UAnimMontage* StunMontage`, `UAnimMontage* DeathMontage`, `DeathMontageFinishTimeShift`, `DeathMontageTimerHandle`, `MeshRelativeTransform`.
  - State: `bool bIsInFocusMode`.
  - Teams: replicated `FGenericTeamId TeamID`.
  - AI: `UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent`.

## UPA_CharacterDefination
Files: `PA_CharacterDefination.h/.cpp`

- Class: `UPA_CharacterDefination : UPrimaryDataAsset`
- Purpose
  - Defines a character entry for selection/spawn menus with display name, icon, class, display anim BP, display mesh, and ability loadout mapping.
- Key Methods
  - Primary Asset: `GetPrimaryAssetId() const`, `static GetCharacterDefinationAssetType()`.
  - Display loading: `LoadIcon() const`, `LoadDisplayAnimationBP() const`, `LoadDisplayMesh() const`.
  - Character loading: `LoadCharacterClass() const`.
  - Abilities: `GetAbilities() const` returns `TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>>`.
- Properties
  - `FString CharacterName`, `TSoftObjectPtr<UTexture2D> CharacterIcon`, `TSoftClassPtr<ACCharacter> CharacterClass`, `TSoftClassPtr<UAnimInstance> DisplayAnimBP`.

## Cross-cutting Integrations
- GAS (Gameplay Ability System)
  - ACCharacter centralizes tag and attribute reactions; can relay gameplay events to self via server RPC for authoritative ability handling.
- Teams
  - ACCharacter exposes `TeamID` for AI and gameplay attitude checks; replicated with `OnRep_TeamID`.
- UI/Widgets
  - Overhead status widget shows health/mana/etc. and is auto-hidden based on range; useful for multiplayer visibility control.
- AI Perception
  - Characters act as perception stimuli sources; AI controllers can sense them via sight and react accordingly.
- Animation
  - Stun/Death montages coordinate visual state with gameplay; ragdoll toggling for physics death.

## Typical Character Flow
1. On spawn/possess, ACCharacter runs server/client init, binds GAS delegates, and sets up UI and perception stimuli.
2. During gameplay, tag and attribute changes drive stun, aim/focus, movement tuning, and health/mana updates.
3. On death: stop relevant logic, play death montage, toggle ragdoll as needed, then respawn or clean up.
4. Team changes replicate and propagate to AI/other systems via `OnRep_TeamID`.

## Extension Points & Notes
- Add more tag listeners (e.g., Stealth, Shielded) and corresponding animation/FX.
- Extend overhead UI to display status effects or interaction prompts.
- Provide blueprint-exposed hooks for death/stun to allow custom montage/FX per character.
- Expand `UPA_CharacterDefination` to include stats, starting equipment, or cosmetic sets.
