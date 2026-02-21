// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MenuPlayerController.h"
// ----------------------------------------------------------------------------
// File: LobbyPlayerController.h
// Purpose: PlayerController used in lobby flow. Issues server requests to
//          change slots, start hero selection, and request match start. Also
//          exposes a client event to transition the UI to hero selection.
// ----------------------------------------------------------------------------
#include "LobbyPlayerController.generated.h"


DECLARE_DELEGATE(FOnSwitchToHeroSelection);
/**
 * Lobby-specific controller extending `AMenuPlayerController` with RPCs for
 * slot selection changes, hero selection start, and match start requests.
 * Provides a local delegate `OnSwitchToHeroSelection` to drive UI transitions.
 */
UCLASS()
class ALobbyPlayerController : public AMenuPlayerController
{
    GENERATED_BODY()
public:
    ALobbyPlayerController();
    FOnSwitchToHeroSelection OnSwitchToHeroSelection;
    /** Ask server to switch current player's lobby slot */
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_RequestSlotSelectionChange(uint8 NewSlotID);

    /** Ask server to begin hero selection phase */
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_StartHeroSelection();

    /** Ask server to request match start (if conditions satisfied) */
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_RequestStartMatch();

    /** Server instructs client to switch to hero selection UI */
    UFUNCTION(Client, Reliable)
    void Client_StartHeroSelection();

};
