// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: InventoryWidget.h
// Purpose: Main inventory panel widget. Displays item grid/slots, supports
//          drag & drop, context menu actions, and selection highlighting.
// ----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryItem.h"
#include "InventoryWidget.generated.h"

// Forward declarations
class UInventoryContextMenuWidget;
class UInventoryItemWidget;

/**
 * Hosts inventory UI elements and forwards user interactions to the
 * `UInventoryComponent`. Usually embedded in `UGameplayWidget`.
 */
UCLASS()
class UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;

    UFUNCTION(BlueprintImplementableEvent)
    void RefreshInventory();

    UFUNCTION(BlueprintImplementableEvent)
    void ShowContextMenu(class UInventoryItemWidget* SourceItemWidget);
private:
    /**
     * Class of the context menu widget to spawn.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TSubclassOf<UInventoryContextMenuWidget> ContextMenuWidgetClass;

    /**
     * The currently spawned context menu widget.
     */
    UPROPERTY()
    UInventoryContextMenuWidget* ContextMenuWidget;

	void SpawnContextMenu();

	UFUNCTION()
	void SellFocusedItem();

	UFUNCTION()
	void UseFocusedItem();

	void SetContextMenuVisible(bool bContextMenuVisible);
	void ToggleContextMenu(const FInventoryItemHandle& ItemHandle);
	void ClearContextMenu();
	FInventoryItemHandle CurrentFocusedItemHandle;


	UPROPERTY(meta=(BindWidget))
	class UWrapBox* ItemList;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInventoryItemWidget> ItemWidgetClass;

	UPROPERTY()
	class UInventoryComponent* InventoryComponent;

	TArray<UInventoryItemWidget*> ItemWidgets;
	TMap<FInventoryItemHandle, UInventoryItemWidget*> PopulatedItemEntryWidgets;

	void ItemAdded(const UInventoryItem* InventoryItem);
	void ItemStackCountChanged(const FInventoryItemHandle& Handle, int NewCount);

	UInventoryItemWidget* GetNextAvaliableSlot() const;

	void HandleItemDragDrop(UInventoryItemWidget* DestinationWidget, UInventoryItemWidget* SourceWidget);
	void ItemRemoved(const FInventoryItemHandle& ItemHandle);
	void ItemAbilityCommitted(const FInventoryItemHandle& ItemHandle, float CooldownDuration, float CooldownTimeRemaining);
};
