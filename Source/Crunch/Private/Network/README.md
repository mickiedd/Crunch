# Network Subsystem Documentation

Updated: 2025-08-14

This document summarizes the networking/session utilities and session management classes under `Source/Crunch/Private/Network`.

Contents
- ACGameSession (session authority)
- UCNetStatics (network/session helpers)
- Integration Points
- Typical Flows
- Extension Notes

## ACGameSession
Files: `CGameSession.h/.cpp`

- Purpose: Custom `AGameSession` that integrates with the Online Subsystem (OSS) for player registration and optional auto-login. Runs on the server/host.
- Responsibilities
  - Handle headless/server auto-login via command-line/ini and signal readiness to host or join sessions.
  - Register/unregister players with the session, keeping OSS state in sync (capacity, invites, matchmaking visibility).
- Key Methods
  - `ProcessAutoLogin()` — Called on server to trigger auto-login at startup. Returns whether the flow was started/handled.
  - `RegisterPlayer(APlayerController*, FUniqueNetIdRepl, bool)` — Notifies OSS when a player has joined, including invite-origin info.
  - `UnregisterPlayer(FName FromSessionName, const FUniqueNetIdRepl&)` — Notifies OSS that a player left.

## UCNetStatics
Files: `CNetStatics.h/.cpp`

- Purpose: Blueprint-accessible library for common OSS-related tasks, standardized session settings, and reading command-line parameters.
- Responsibilities
  - Provide safe access to `IOnlineSession` and `IOnlineIdentity` pointers.
  - Generate consistent `FOnlineSessionSettings` according to project conventions.
  - Centralize key names/values (session name, search id, port, coordinator URL, testing URL) and expose getters for C++/Blueprints.
  - Convenience helpers to detect server/host role and to parse command-line args.
- Key Methods
  - `GenerateOnlineSesisonSettings(SessionName, SessionSearchId, Port)` — Builds settings (public lobbies, presence, allow join in progress, etc. as per project defaults).
  - `GetSessionPtr()`, `GetIdentityPtr()` — Retrieve OSS interfaces.
  - `GetPlayerCountPerTeam()` — Returns configured team size.
  - `IsSessionServer(WorldContext)` — True if current instance is acting as the host/server.
  - Keys/Values: `GetSessionNameStr/Key`, `GetSesisonSearchIdStr/Key`, `GetSessionPort()/GetPortKey()`, `GetCoordinatorURL()/Key`, `GetDefaultCoordinatorURL()`, `GetTestingURL()/Key`.
  - Command line: `GetCommandlineArgAsString(Name)`, `GetCommandlineArgAsInt(Name)`.
  - Utils: `ReplacePort(OutURLStr, NewPort)`.

## Integration Points
- Framework/Game Modes
  - Game modes and instance startup can call `ProcessAutoLogin()` or use `UCNetStatics` to set up session settings and read CLI params.
- Online Subsystem (OSS)
  - `ACGameSession` methods ensure player membership is propagated to the active session; `UCNetStatics` provides portable keys and helpers.
- Dedicated/Listen Servers
  - `IsSessionServer` and command-line parsing help configure dedicated vs. listen hosting behaviors (ports, coordinator URLs).

## Typical Flows
1. Server startup with auto-login
   - Server boots with CLI flags (e.g., `-Port=7777 -SessionName=CrunchSession`).
   - `ACGameSession::ProcessAutoLogin()` runs, authenticates, and then creates/advertises a session using settings from `UCNetStatics::GenerateOnlineSesisonSettings(...)`.
2. Player joins/leaves
   - On join: `RegisterPlayer(...)` informs OSS; capacity/visibility are updated.
   - On leave: `UnregisterPlayer(...)` updates OSS membership and cleans up.
3. Client connect helpers
   - Client reads coordinator/testing URL and port via `UCNetStatics`, constructs a travel URL (optionally `ReplacePort`) and connects.

## Extension Notes
- Session Settings Profiles
  - Add multiple presets (ranked/unranked/private) in `GenerateOnlineSesisonSettings` controlled by CLI/ini.
- Crossplay and Presence
  - Extend identity/session accessors to surface platform-specific flags (crossplay, presence, LAN, invites).
- Matchmaking Integration
  - Add helpers to create/find sessions with filter keys, and to wrap async results into Blueprint-friendly delegates.
- Telemetry
  - Hook registration events to analytics (join reasons, region, latency).
