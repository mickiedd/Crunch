// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/PlayerInfoTypes.h"
#include "LobbyWidget.generated.h"

/**
 * Lobby screen that switches between Team Selection and Hero Selection views,
 * populates team slots, shows other players, and issues start requests.
 */
UCLASS()
class ULobbyWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    /**
     * NativeConstruct is called when the widget is initialized.
     */
    virtual void NativeConstruct() override;
private: 
    /**
     * Main widget switcher for Team Selection and Hero Selection views.
     */
    UPROPERTY(meta=(BindWidget))
    class UWidgetSwitcher* MainSwitcher;

    /**
     * Root widget for Team Selection view.
     */
    UPROPERTY(meta=(BindWidget)) 
    class UWidget* TeamSelectionRoot;

    /**
     * Button to start Hero Selection.
     */
    UPROPERTY(meta=(BindWidget))
    class UButton* StartHeroSelectionButton;

    /**
     * Grid panel for Team Selection slots.
     */
    UPROPERTY(meta=(BindWidget))
    class UUniformGridPanel* TeamSelectionSlotGridPanel;

    /**
     * Class of Team Selection widget.
     */
    UPROPERTY(EditDefaultsOnly, Category = "TeamSelection")
    TSubclassOf<class UTeamSelectionWidget> TeamSelectionWidgetClass;

    /**
     * Array of Team Selection widgets.
     */
    UPROPERTY()
    TArray<class UTeamSelectionWidget*> TeamSelectionSlots;

    /**
     * Clears and populates Team Selection slots.
     */
    void ClearAndPopulateTeamSelectionSlots();

    /**
     * Handles slot selection.
     * @param NewSlotID ID of the selected slot.
     */
    void SlotSelected(uint8 NewSlotID);

    /**
     * Root widget for Hero Selection view.
     */
    UPROPERTY(meta=(BindWidget)) 
    class UWidget* HeroSelectionRoot;

    /**
     * Tile view for character selection.
     */
    UPROPERTY(meta=(BindWidget)) 
    class UTileView* CharacterSelectionTileView;

    /**
     * List view for abilities.
     */
    UPROPERTY(meta=(BindWidget)) 
    class UAbilityListView* AbilityListView;

    /**
     * Widget for player team layout.
     */
    UPROPERTY(meta=(BindWidget)) 
    class UPlayerTeamLayoutWidget* PlayerTeamLayoutWidget;

    /**
     * Button to start the match.
     */
    UPROPERTY(meta=(BindWidget)) 
    class UButton* StartMatchButton;

    /**
     * Lobby player controller.
     */
    UPROPERTY()
    class ALobbyPlayerController* LobbyPlayerController;

    /**
     * Player state.
     */
    UPROPERTY()
    class ACPlayerState* CPlayerState;

    /**
     * Configures game state.
     */
    void ConfigureGameState();

    /**
     * Timer handle for ConfigureGameState.
     */
    FTimerHandle ConfigureGameStateTimerHandle;

    /**
     * Game state.
     */
    UPROPERTY()
    class ACGameState* CGameState;

    /**
     * Updates player selection display.
     * @param PlayerSelections Array of player selections.
     */
    void UpdatePlayerSelectionDisplay(const TArray<FPlayerSelection>& PlayerSelections);

    /**
     * Handles Start Hero Selection button click.
     */
    UFUNCTION()
    void StartHeroSelectionButtonClicked();

    /**
     * Switches to Hero Selection view.
     */
    void SwitchToHeroSelection();

    /**
     * Handles character definition loading.
     */
    void CharacterDefinationLoaded();

    /**
     * Handles character selection.
     * @param SelectedUObject Selected character object.
     */
    void CharacterSelected(UObject* SelectedUObject);

    /**
     * Class of character display widget.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Character Display")
    TSubclassOf<class ACharacterDisplay> CharacterDisplayClass;

    /**
     * Character display widget.
     */
    UPROPERTY()
    class ACharacterDisplay* CharacterDisplay;

    /**
     * Spawns character display.
     */
    void SpawnCharacterDisplay();

    /**
     * Updates character display.
     * @param PlayerSelection Player selection.
     */
    void UpdateCharacterDisplay(const FPlayerSelection& PlayerSelection);

    /**
     * Handles Start Match button click.
     */
    UFUNCTION()
    void StartMatchButtonClicked();
};
