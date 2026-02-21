// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// ----------------------------------------------------------------------------
// File: TeamSelectionWidget.h
// Purpose: Clickable slot used on the lobby team selection screen. Displays
//          occupant info and emits a delegate when clicked.
// ----------------------------------------------------------------------------
#include "TeamSelectionWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, uint8 /*SlotID*/);
/**
 * Represents a single team slot in the lobby UI. It updates with the current
 * player's nickname occupying the slot and notifies listeners when clicked to
 * request selection.
 */
UCLASS()
class UTeamSelectionWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    /** Set the numerical slot ID represented by this widget */
    void SetSlotID(uint8 NewSlotID);
    /** Update the display text using the provided player's nickname */
    void UpdateSlotInfo(const FString& PlayerNickName);
    virtual void NativeConstruct() override;

    /** Emitted when user clicks the select button; carries SlotID */
    FOnSlotClicked OnSlotClicked;
private: 
    /** Button that triggers selection of this slot */
    UPROPERTY(meta=(BindWidget))
    class UButton* SelectButton;

    /** Text showing either empty or the occupant's nickname */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* InfoText;

    UFUNCTION()
    void SelectButtonClicked();

    uint8 SlotID;
};
