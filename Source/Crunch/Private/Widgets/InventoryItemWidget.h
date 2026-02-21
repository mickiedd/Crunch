// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/ItemWidget.h"
#include "Inventory/InventoryItem.h"
#include "InventoryItemWidget.generated.h"

class UInventoryItem;
class UInventoryItemWidget;

/**
 * Delegate for when an inventory item is dropped onto another widget.
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemDropped, UInventoryItemWidget* /*DestionationWidget*/, UInventoryItemWidget* /*SourceWidget*/);

/**
 * Delegate for when a button is clicked on an inventory item.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnButtonClick, const FInventoryItemHandle& /*ItemHandle*/);

/**
 * Inventory slot widget that presents an `UInventoryItem` and exposes user
 * interactions via delegates. Handles drag/drop to move/combine items and
 * displays cooldown progression with a material parameter-driven overlay.
 */
UCLASS()
class UInventoryItemWidget : public UItemWidget
{
    GENERATED_BODY()

public:
    /**
     * Delegate for when an inventory item is dropped onto another widget.
     */
    FOnInventoryItemDropped OnInventoryItemDropped;

    /**
     * Delegate for when the left button is clicked on an inventory item.
     */
    FOnButtonClick OnLeftBttonClicked;

    /**
     * Delegate for when the right button is clicked on an inventory item.
     */
    FOnButtonClick OnRightBttonClicked;

    /**
     * Native construct event.
     */
    virtual void NativeConstruct() override;

    /**
     * Checks if the inventory slot is empty.
     *
     * @return True if the slot is empty, false otherwise.
     */
    bool IsEmpty() const;

    /**
     * Sets the slot number for this inventory item.
     *
     * @param NewSlotNumber The new slot number.
     */
    void SetSlotNumber(int NewSlotNumber);

    /**
     * Updates the inventory item being displayed.
     *
     * @param Item The new inventory item.
     */
    void UpdateInventoryItem(const UInventoryItem* Item);

    /**
     * Empties the inventory slot.
     */
    void EmptySlot();

    /**
     * Gets the current slot number.
     *
     * @return The current slot number.
     */
    FORCEINLINE int GetSlotNumber() const { return SlotNumber; }

    /**
     * Updates the stack count display.
     */
    void UpdateStackCount();

    /**
     * Gets the icon texture for the current inventory item.
     *
     * @return The icon texture.
     */
    UTexture2D* GetIconTexture() const;

    /**
     * Gets the current inventory item.
     *
     * @return The current inventory item.
     */
    FORCEINLINE const UInventoryItem* GetInventoryItem() const { return InventoryItem; }

    /**
     * Gets the item handle for the current inventory item.
     *
     * @return The item handle.
     */
    FInventoryItemHandle GetItemHandle() const;

private:

    /**
     * Updates the can cast display based on the current cooldown state.
     *
     * @param bCanCast Whether the item can be cast.
     */
    void UpdateCanCastDisplay(bool bCanCast);

    /**
     * The texture to display when the slot is empty.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    UTexture2D* EmptyTexture;

    /**
     * The text block for displaying the stack count.
     */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* StackCountText;

    /**
     * The text block for displaying the cooldown count.
     */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* CooldownCountText;

    /**
     * The text block for displaying the cooldown duration.
     */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* CooldownDurationText;

    /**
     * The text block for displaying the mana cost.
     */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* ManaCostText;

    /**
     * The current inventory item.
     */
    UPROPERTY()
    const UInventoryItem* InventoryItem;

    /**
     * The current slot number.
     */
    int SlotNumber;

    /**
     * Called when the right button is clicked.
     */
    virtual void RightButtonClicked() override;

    /**
     * Called when the left button is clicked.
     */
    virtual void LeftButtonClicked() override;

    /******************************************/
    /*           Drag Drop                    */
    /******************************************/

private:
    /**
     * Called when a drag operation is detected.
     *
     * @param InGeometry The geometry of the drag operation.
     * @param InMouseEvent The mouse event that triggered the drag operation.
     * @param OutOperation The drag operation to perform.
     */
    virtual void NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation ) override;

    /**
     * Called when a drop operation is performed.
     *
     * @param InGeometry The geometry of the drop operation.
     * @param InDragDropEvent The drag drop event that triggered the drop operation.
     * @param InOperation The drop operation to perform.
     *
     * @return True if the drop operation was successful, false otherwise.
     */
    virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;

    /**
     * The class to use for drag drop operations.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Drag Drop")
    TSubclassOf<class UInventoryItemDragDropOp> DragDropOpClass;

    /******************************************/
    /*            GAS                         */
    /******************************************/

public:
    /**
     * Starts the cooldown overlay tick/update for an ability item.
     *
     * @param CooldownDuration The duration of the cooldown.
     * @param TimeRemaining The remaining time on the cooldown.
     */
    void StartCooldown(float CooldownDuration, float TimeRemaining);

private:
    /**
     * The interval at which to update the cooldown display.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
    float CooldownUpdateInterval = 0.1f;

    /**
     * Binds the can cast ability delegate.
     */
    void BindCanCastAbilityDelegate();

    /**
     * Unbinds the can cast ability delegate.
     */
    void UnBindCanCastAbilityDelegate();

    /**
     * Called when the cooldown finishes.
     */
    void CooldownFinished();

    /**
     * Updates the cooldown display.
     */
    void UpdateCooldown();

    /**
     * Clears the cooldown display.
     */
    void ClearCooldown();

    /**
     * The timer handle for the cooldown duration.
     */
    FTimerHandle CooldownDurationTimerHandle;

    /**
     * The timer handle for the cooldown update.
     */
    FTimerHandle CooldownUpdateTimerHandle;

    /**
     * The remaining time on the cooldown.
     */
    float CooldownTimeRemaining = 0.f;

    /**
     * The duration of the cooldown.
     */
    float CooldownTimeDuration = 0.f;

    /**
     * The material parameter name for the cooldown amount.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
    FName CooldownAmtDynamicMaterialParamName = "Percent";

    /**
     * The material parameter name for the icon texture.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
    FName IconTextureDynamicMaterialParamName = "Icon";

    /**
     * The material parameter name for the can cast indicator.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
    FName CanCastDynamicMaterialParamName = "CanCast";

    /**
     * Sets the icon texture for the current inventory item.
     *
     * @param IconTexture The new icon texture.
     */
    virtual void SetIcon(UTexture2D* IconTexture) override;

    /**
     * The formatting options for the cooldown display.
     */
    FNumberFormattingOptions CooldownDisplayFormattingOptions;
};
