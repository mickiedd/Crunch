// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: CPlayerState.h
// Purpose: Replicated player state holding selection info (slot, chosen hero),
//          exposing team id based on slot, and relaying selection updates from
//          server game state. Used across lobby and match.
// ----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Player/PlayerInfoTypes.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayerState.generated.h"


class UPA_CharacterDefination;
/**
 * Extends `APlayerState` to carry lobby/match selection data and provide a
 * derived team id from the current slot. Handles replication and initialization
 * of selection data and copying on seamless travel.
 */
UCLASS()
class ACPlayerState : public APlayerState
{
    GENERATED_BODY()
public:
    ACPlayerState();
    /**
     * Called to get the list of properties that are replicated.
     *
     * @param OutLifetimeProps The list of properties that are replicated.
     */
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
    /**
     * Called when the game starts or when the player is spawned.
     */
    virtual void BeginPlay() override;
    /**
     * Called to copy properties from another player state.
     *
     * @param PlayerState The player state to copy properties from.
     */
    virtual void CopyProperties(APlayerState* PlayerState) override;
    /**
     * Gets the selected pawn class based on the current selection.
     *
     * @return The selected pawn class.
     */
    TSubclassOf<APawn> GetSelectedPawnClass() const;
    /**
     * Gets the team id based on the current slot.
     *
     * @return The team id.
     */
    FGenericTeamId GetTeamIdBasedOnSlot() const;
    /**
     * Sets the selected character definition on the server.
     *
     * @param NewDefination The new character definition.
     */
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SetSelectedCharacterDefination(const UPA_CharacterDefination* NewDefination);

private: 
    /**
     * The player's current selection.
     */
    UPROPERTY(Replicated)
    FPlayerSelection PlayerSelection;

    /**
     * The game state.
     */
    UPROPERTY() 
    class ACGameState* CGameState;

    /**
     * Called when the player's selection is updated.
     *
     * @param NewPlayerSelections The new player selections.
     */
    void PlayerSelectionUpdated(const TArray<FPlayerSelection>& NewPlayerSelections);
};
