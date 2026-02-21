// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/CGameMode.h"
// ----------------------------------------------------------------------------
// File: LobbyGameMode.h
// Purpose: Specialized GameMode for the lobby/hero selection phase. Relies on
//          ACGameMode's team spawn logic while deferring actual match start
//          until lobby conditions are satisfied.
// ----------------------------------------------------------------------------
#include "LobbyGameMode.generated.h"

/**
 * Lightweight extension of ACGameMode used during lobby. Typically pairs with
 * ACGameState to manage player slot/character selection prior to starting the
 * match and seamless travel to the game map.
 */
UCLASS()
class ALobbyGameMode : public ACGameMode
{
	GENERATED_BODY()
public:	
	ALobbyGameMode();
};
