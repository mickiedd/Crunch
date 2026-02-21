// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/PlayerInfoTypes.h"
// ----------------------------------------------------------------------------
// File: PlayerTeamLayoutWidget.h
// Purpose: Lays out player slots for both teams in the lobby, updating per
//          player selections and creating slot widgets dynamically.
// ----------------------------------------------------------------------------
#include "PlayerTeamLayoutWidget.generated.h"

class UPlayerTeamSlotWidget;
/**
 * Displays two horizontal lists of player slots (team one and team two). On
 * update, ensures slot widgets exist and reflect the provided selections.
 */
UCLASS()
class UPlayerTeamLayoutWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    /** Update the UI with the latest player team/character selections */
    void UpdatePlayerSelection(const TArray<FPlayerSelection>& PlayerSelections);
private: 
    /** Margin between player slot widgets */
    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    float PlayerTeamWidgetSlotMargin = 5.f;

    /** Slot widget class spawned for each player */
    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    TSubclassOf<UPlayerTeamSlotWidget> PlayerTeamSlotWidgetClass;

    UPROPERTY(meta=(BindWidget))
    class UHorizontalBox* TeamOneLayoutBox;

    UPROPERTY(meta=(BindWidget))
    class UHorizontalBox* TeamTwoLayoutBox;

    /** Cache of constructed slot widgets for quick updates */
    UPROPERTY()
    TArray<UPlayerTeamSlotWidget*> TeamSlotWidgets;
};
