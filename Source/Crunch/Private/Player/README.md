# Player Subsystem

This folder contains Player-focused classes: controllers for gameplay, lobby, and main menu; the local player character; the replicated player state; and lightweight info types used across menus and match.

Contents
- CPlayerController.h/.cpp: In-game PlayerController implementing team agent, UI, input, and match-finish flow.
- CPlayerCharacter.h/.cpp: Player-controlled character bridging camera, enhanced input, GAS ability inputs, and inventory usage.
- CPlayerState.h/.cpp: Replicated state carrying lobby/match player selection and exposing team id derived from slot.
- MenuPlayerController.h/.cpp: Minimal base PC that spawns a menu widget; used by lobby and main menu.
- LobbyPlayerController.h/.cpp: Lobby-specific controller with RPCs for slot change, start hero selection, and request start match.
- MainMenuPlayerController.h/.cpp: Thin wrapper over MenuPlayerController for the main menu map.
- PlayerInfoTypes.h/.cpp: Data types like FPlayerSelection used for lobby synchronization.

## ACPlayerController (in-game)
Responsibilities
- Possession lifecycle: server `OnPossess`, client `AcknowledgePossession` to wire character, inputs, and widgets.
- Team integration: implements IGenericTeamAgentInterface; replicates `TeamID` with `GetLifetimeReplicatedProps`.
- UI: spawns and owns `UGameplayWidget` on start; toggles shop and gameplay menu via enhanced input.
- Match finish: `MatchFinished` triggers smooth camera/view transition; mirrored to clients via `Client_MatchFinished`.

Key Properties
- `FGenericTeamId TeamID` (Replicated) — used by AI/team-based logic.
- `UGameplayWidget* GameplayWidget`, `TSubclassOf<UGameplayWidget> GameplayWidgetClass` — in-game HUD.
- Input: `UInputMappingContext* UIInputMapping`, `UInputAction* ShopToggleInputAction`, `UInputAction* ToggleGameplayMenuAction`.

Key Methods
- `OnPossess`, `AcknowledgePossession`, `SetupInputComponent` — lifecycle and input hookup.
- `SetGenericTeamId`, `GetGenericTeamId` — team API.
- `MatchFinished`, `Client_MatchFinished`, `ShowWinLoseState` — end-of-match flow.

Integration
- Enhanced Input for UI toggles.
- HUD/widgets via UMG. Team system for AI perception/friendly-fire logic.

## ACPlayerCharacter (local player pawn)
Responsibilities
- Camera rig: spring arm + camera; supports aim-offset via local camera offsets with smooth lerp.
- Enhanced Input: movement, look, jump; forwards mapped `ECAbilityInputID` to GAS ability system.
- Gameplay hooks: aim state (`OnAimStateChanged`), stun (`OnStun`/`OnRecoverFromStun`), death/respawn (`OnDead`/`OnRespawn`).
- Inventory: `UInventoryComponent* InventoryComponent` for using items via input.

Key Properties
- View: `USpringArmComponent* CameraBoom`, `UCameraComponent* ViewCam`.
- Ability: `TMap<ECAbilityInputID, UInputAction*> GameplayAbilityInputActions`, `UInputMappingContext* GameplayInputMappingContext`.
- Attributes: `UCHeroAttributeSet* HeroAttributeSet` (read-only ref for local UI/logic).
- Camera offsets: `FVector CameraAimLocalOffset`, `float CamerLerpSpeed`, `FTimerHandle CamerLerpTimerHandle`.

Key Methods
- `PawnClientRestart`, `SetupPlayerInputComponent` — re-add input mappings on restart; bind actions/axes.
- `GetActorEyesViewPoint` — camera source for traces/abilities.
- Input handlers: `HandleLookInput`, `HandleMoveInput`, `HandleAbilityInput`, `UseInventoryItem`, learn ability leader up/down.

Integration
- GAS input routing using `ECAbilityInputID` from `CGameplayAbilityTypes.h`.
- Inventory subsystem for item usage.

## ACPlayerState
Responsibilities
- Replicates `FPlayerSelection` across lobby and match.
- Computes team identity based on lobby slot: `GetTeamIdBasedOnSlot`.
- Initializes and keeps `PlayerSelection` in sync with `ACGameState` (via `PlayerSelectionUpdated`).
- Supports seamless travel: `CopyProperties`.

Key Methods
- `GetLifetimeReplicatedProps` — replicates `PlayerSelection`.
- `Server_SetSelectedCharacterDefination(const UPA_CharacterDefination*)` — authoritative update of chosen character.
- `GetSelectedPawnClass()` — resolves chosen pawn for spawn.

Integration
- References `UPA_CharacterDefination` (character data asset) from Framework/Character systems.
- Team id interoperates with AI/Gameplay team mechanics via `FGenericTeamId`.

## AMenuPlayerController (base for menus)
Responsibilities
- Spawns and owns a `UUserWidget` specified by `MenuWidgetClass` in `BeginPlay` and `OnRep_PlayerState`.
- Shares menu bootstrapping logic across main menu and lobby.

Key Properties
- `TSubclassOf<UUserWidget> MenuWidgetClass`, `UUserWidget* MenuWidget`.

## ALobbyPlayerController
Responsibilities
- Server RPCs for lobby flow: change slot, start hero selection, request starting the match.
- Client notification to transition UI to hero selection.
- Exposes `FOnSwitchToHeroSelection` for local UI code.

Key Methods
- `Server_RequestSlotSelectionChange(uint8)` — select a different lobby slot.
- `Server_StartHeroSelection()` — begin hero selection phase.
- `Server_RequestStartMatch()` — ask to start match (subject to server rules).
- `Client_StartHeroSelection()` — client moves UI into hero selection.

## AMainMenuPlayerController
Responsibilities
- Thin specialization of `AMenuPlayerController` for the main menu map.

## PlayerInfoTypes (FPlayerSelection)
Responsibilities
- Encapsulates the local player’s lobby state: slot, `FUniqueNetIdRepl`, nickname, and selected `UPA_CharacterDefination`.
- Utility: `IsForPlayer`, `IsValid`, `GetInvalidSlot`.

Fields
- `uint8 Slot` — lobby slot id.
- `FUniqueNetIdRepl PlayerUniqueId` — OSS-compatible net id.
- `FString PlayerNickName` — display name.
- `const UPA_CharacterDefination* CharacterDefination` — chosen hero data.

## Typical Flows
- Join Lobby
  1) Player joins lobby map; `ALobbyPlayerController` spawns menu widget.
  2) `ACPlayerState` initializes `FPlayerSelection`; UI shows slots.
  3) Player requests slot change via `Server_RequestSlotSelectionChange`.

- Start Hero Selection
  1) Host calls `Server_StartHeroSelection`.
  2) Server replicates state; clients receive `Client_StartHeroSelection` and switch UI.
  3) Clients set `Server_SetSelectedCharacterDefination` on `ACPlayerState`.

- Enter Match
  1) Map loads; `ACPlayerController` possesses `ACPlayerCharacter`.
  2) `SetupInputComponent` binds UI toggles and ability inputs; HUD spawns.
  3) `TeamID` replicated; AI/team logic uses `IGenericTeamAgentInterface`.
  4) On match end, `MatchFinished` triggers camera/view blending and win/lose UI.

## Extension Notes
- Add more Enhanced Input actions by extending `GameplayAbilityInputActions` and mapping in the input context.
- Expose additional data in `FPlayerSelection` (e.g., skin, emotes) with proper replication in `ACPlayerState`.
- Consider authority checks in RPC implementations and UI validation for lobby actions.
- Ensure `UPA_CharacterDefination` to pawn class mapping stays authoritative on the server.
