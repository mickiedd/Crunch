// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
// ----------------------------------------------------------------------------
// File: InventoryContextMenuWidget.h
// Purpose: Right-click context menu for inventory items with Use/Sell actions.
//          Exposes button clicked events for external binding.
// ----------------------------------------------------------------------------
#include "InventoryContextMenuWidget.generated.h"

/**
 * Small utility menu shown for an inventory item to trigger actions. It does
 * not perform logic itself; consumers bind to the button click events and
 * route to InventoryComponent or shop logic as appropriate.
 */
UCLASS()
class UInventoryContextMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** Accessors for the underlying button click events */
    FOnButtonClickedEvent& GetSellButtonClickedEvent() const;
    FOnButtonClickedEvent& GetUseButtonClickedEvent() const;

private:
    /** Use the item (consume/equip/activate) */
    UPROPERTY(meta=(BindWidget))
    UButton* UseButton;

    /** Sell the item (when in shop context) */
    UPROPERTY(meta=(BindWidget))
    UButton* SellButton;

};
