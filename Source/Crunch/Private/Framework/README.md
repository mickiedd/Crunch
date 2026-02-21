# Framework Subsystem Documentation

Updated: 2025-08-13

This document interprets classes under `Source/Crunch/Private/Framework` and explains responsibilities, key APIs, properties, and how they integrate with other subsystems (GAS, Teams, UI, Online Sessions).

Contents
- UCAssetManager
- UCGameInstance
- ACGameMode
- ACGameState
- ALobbyGameMode
- AMainMenuGameMode
- AStormCore
- Typical Flows
- Extension Notes

## UCAssetManager
Files: `CAssetManager.h/.cpp`

- Purpose: Centralized async loading and runtime queries for content referenced by soft pointers.
- Responsibilities
  - Load and cache Character Definitions (`UPA_CharacterDefination`) and Shop Items (`UPA_ShopItem`).
  - Build fast lookup maps for item combination/ingredient relationships.
- Key Methods
  - `static Get()` accessor for the singleton instance.
  - `LoadCharacterDefinations(const FStreamableDelegate& Callback)`; `GetLoadedCharacterDefinations(...)`.
  - `LoadShopItems(const FStreamableDelegate& Callback)`; `GetLoadedShopItems(...)`.
  - `GetCombinationForItem(...)`, `GetIngredientForItem(...)` using internal maps.
- Properties
  - `CombinationMap`, `IngredientMap`: `TMap<const UPA_ShopItem*, FItemCollection>` built after load.

## UCGameInstance
Files: `CGameInstance.h/.cpp`

- Purpose: Orchestrates login, online session lifecycle (create/search/join), and listen-server level travel.
- Responsibilities
  - Login: account portal login, completion delegate (`FOnLoginCompleted`).
  - Client session: request server creation, poll for discoverability, global search, join by Id.
  - Server session: host session, track players joined/left, enforce timeouts.
  - Level travel: soft references to `MainMenu`, `Lobby`, and `Game` maps; `LoadLevelAndListen`.
- Key Methods/Delegates
  - `ClientAccountPortalLogin()`, `IsLoggedIn()`, `IsLoggingIn()`; `OnLoginCompleted`.
  - `RequestCreateAndJoinSession(...)`, `StartGlobalSessionSearch()`, `JoinSessionWithId(...)`.
  - `OnJoinSessionFailed`, `OnGlobalSessionSearchCompleted`.
  - Server-side: `CreateSession()`, `OnSessionCreated(...)`, `PlayerJoined/Left(...)`, `EndSessisonCompleted(...)`.
  - Timers: `FindCreatedSesisonTimerHandle`, `GlobalSessionSearchTimerHandle`, timeout durations.
- Properties
  - Map soft refs: `MainMenuLevel`, `LobbyLevel`, `GameLevel`.
  - Session intervals/timeouts: `GlobalSessionSearchInterval`, `FindCreatedSessionSearchInterval`, `FindCreatedSessionTimeoutDuration`, `WaitPlayerJoinTimeOutDuration`.

## ACGameMode
Files: `CGameMode.h/.cpp`

- Purpose: Match coordination, team-aware spawn selection, default pawn choice, and match finish.
- Responsibilities
  - Override spawning points based on `FGenericTeamId` to `PlayerStart` tag mapping.
  - Provide fallback pawn class when default pawn cannot be resolved.
  - Integrate with `AStormCore` to determine victory and notify clients.
- Key Methods
  - `SpawnPlayerController(...)`, `StartPlay()`.
  - `GetDefaultPawnClassForController_Implementation(...)`.
  - `SpawnDefaultPawnFor_Implementation(...)`.
  - `MatchFinished(AActor* ViewTarget, int WiningTeam)`.
- Properties
  - `BackupPawn`: fallback pawn.
  - `TeamStartSpotTagMap`: team-to-tag mapping for spawn selection.

## ACGameState
Files: `CGameState.h/.cpp`

- Purpose: Replicated state for lobby/hero selection prior to match start.
- Responsibilities
  - Tracks `TArray<FPlayerSelection>`; enforces slot occupancy and unique character selection.
  - Replicates selection array and notifies via `OnPlayerSelectionUpdated`.
  - Validates readiness: `CanStartHeroSelection()`, `CanStartMatch()`.
- Key Methods
  - `RequestPlayerSelectionChange(const APlayerState*, uint8 DesiredSlot)`.
  - `SetCharacterSelected(const APlayerState*, const UPA_CharacterDefination*)`.
  - `SetCharacterDeselected(const UPA_CharacterDefination*)`.
  - `IsSlotOccupied(uint8) const`, `IsDefiniationSelected(const UPA_CharacterDefination*) const`.

## ALobbyGameMode
Files: `LobbyGameMode.h/.cpp`

- Purpose: Lobby-phase game mode leveraging `ACGameMode` spawning/team logic while awaiting player readiness.
- Notes: Typically used with `ACGameState` to manage slot/character selection and transition to game map.

## AMainMenuGameMode
Files: `MainMenuGameMode.h/.cpp`

- Purpose: Minimal front-end host. Generally no spawn/match logic; widgets and UI flows only.

## AStormCore
Files: `StormCore.h/.cpp`

- Purpose: Moving objective character controlled by team influence in an area.
- Responsibilities
  - Count nearby influencers per team within `InfluenceRadius` sphere; compute team weight.
  - Select `CoreToCapture` and move/expand toward team goals; replicate state.
  - Notify listeners via `OnGoalReachedDelegate` and influence updates via `OnTeamInfluenceCountUpdated`.
- Key Methods/Events
  - Overlap handlers: `NewInfluenerInRange(...)`, `InfluencerLeftRange(...)`.
  - State updates: `UpdateTeamWeight()`, `UpdateGoal()`, `OnRep_CoreToCapture()`.
  - Actions: `CaptureCore()`, `ExpandFinished()`, `GoalReached(int WiningTeam)`.
- Properties
  - Visuals: `ExpandMontage`, `CaptureMontage`, `GroundDecalComponent`, `ViewCam`.
  - Detection: `InfluenceRange` sphere, `InfluenceRadius`, `MaxMoveSpeed`.
  - Team targets: `TeamOneGoal`, `TeamTwoGoal`, `TeamOneCore`, `TeamTwoCore`.
  - Replication: `CoreToCapture`, progress via `GetProgress()`.

## Typical Flows
1. Boot → `AMainMenuGameMode` hosts UI. `UCGameInstance` handles login.
2. Create/Join session → travel to Lobby map with `ALobbyGameMode` + `ACGameState`.
3. Players select slots and characters → replicated by `ACGameState`.
4. Start Match → travel to Game map with `ACGameMode`. Teams spawn at tagged PlayerStarts.
5. Objective (`AStormCore`) progresses based on nearby influencers → win → `ACGameMode::MatchFinished`.
6. Return to Lobby or Main Menu as needed.

## Extension Notes
- Asset Manager
  - Add loaders for additional asset types (e.g., cosmetics, maps) and extend composition graphs beyond items.
- Game Instance
  - Harden session retry/backoff; surface richer error codes via delegates.
  - Add crossplay/account-link flows if relevant.
- Game Mode/State
  - Add warmup timers, AFK kick, team balancing, rematch flow.
- Storm Core
  - Expose Blueprints events for FX and audio hooks; add debug draw of influence.
  - Parameterize weights per class/role; add obstruction/line-of-sight checks.
