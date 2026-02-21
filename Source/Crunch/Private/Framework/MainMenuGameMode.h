// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: MainMenuGameMode.h
// Purpose: Minimal GameMode for the main menu level. Typically no spawning or
//          match logic; exists to host UI-only front-end flows.
// ----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * Barebones GameModeBase used by the main menu map. Keeps gameplay systems
 * dormant while allowing widgets and front-end logic to run.
 */
UCLASS()
class AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	
	
	
};
