// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: CGameSession.h
// Purpose: Custom GameSession handling online session player registration and
//          auto-login flow integration. Serves as server-side authority for
//          session membership and ties into OnlineSubsystem.
// ----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "CGameSession.generated.h"

/**
 * Extends AGameSession to plug into the project's Online Subsystem usage.
 * Overrides auto-login and player registration hooks to coordinate session
 * state with backend/OSS.
 */
UCLASS()
class ACGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:	
	// Called on server to perform headless/server auto-login if configured
	// via commandline or ini. Return true if login started/handled.
	virtual bool ProcessAutoLogin() override;
	
	// Track player addition/removal to the active session. Used for capacity,
	// matchmaking, and session visibility in OSS.
	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite) override;
	virtual void UnregisterPlayer(FName FromSessionName, const FUniqueNetIdRepl& UniqueId) override;
};
