// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: MainMenuPlayerController.h
// Purpose: PlayerController used in the main menu. Inherits menu widget
//          bootstrapping from `AMenuPlayerController` and can host any
//          additional main-menu specific logic.
// ----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "Player/MenuPlayerController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * Thin wrapper over `AMenuPlayerController` for the main menu map.
 */
UCLASS()
class AMainMenuPlayerController : public AMenuPlayerController
{
	GENERATED_BODY()
	
	
public:	
	AMainMenuPlayerController();
	
};
