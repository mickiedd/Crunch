// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Widgets/ItemWidget.h"
#include "Widgets/TreeNodeInterface.h"
// ----------------------------------------------------------------------------
// File: ShopItemWidget.h
// Purpose: List entry widget for shop items. Displays an item icon, supports
//          selection and purchase, and participates in item tree visualization
//          via ITreeNodeInterface.
// ----------------------------------------------------------------------------
#include "ShopItemWidget.generated.h"

class UPA_ShopItem;
class UShopItemWidget;

/**
 * Fired when the user confirms purchasing an item.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemPurchaseIssused, const UPA_ShopItem*);

/**
 * Fired when a shop item entry is clicked/selected in the list.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShopItemSelected, const UShopItemWidget*);

/**
 * Shop list item. As a list entry, it reacts to being set with a backing
 * object (UPA_ShopItem) and updates visuals. As a tree node, it exposes its
 * input components and output upgrades for recipe visualization.
 */
UCLASS()
class UShopItemWidget : public UItemWidget, public IUserObjectListEntry, public ITreeNodeInterface
{
    GENERATED_BODY()
public:
    /**
     * Fired when the user confirms purchasing this item.
     */
    FOnItemPurchaseIssused OnItemPurchaseIssued;

    /**
     * Fired when this entry is clicked/selected in the list.
     */
    FOnShopItemSelected OnShopItemClicked;

    // ITreeNodeInterface
    /**
     * Returns the widget associated with this tree node.
     */
    virtual UUserWidget* GetWidget() const override;

    /**
     * Returns the input components of this tree node.
     */
    virtual TArray<const ITreeNodeInterface*> GetInputs() const override;

    /**
     * Returns the output upgrades of this tree node.
     */
    virtual TArray<const ITreeNodeInterface*> GetOuputs() const override;

    /**
     * Returns the object associated with this tree node.
     */
    virtual const UObject* GetItemObject() const override;
    // End ITreeNodeInterface

    // IUserObjectListEntry: called when list item object is assigned
    /**
     * Called when the list item object is assigned to this widget.
     */
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

    /**
     * Returns the shop item associated with this widget.
     */
    FORCEINLINE const UPA_ShopItem* GetShopItem() const { return ShopItem; }
private:
    /**
     * Copies relevant UI state from another widget (e.g., selection).
     */
    void CopyFromOther(const UShopItemWidget* OtherWidget);

    /**
     * Initializes this widget with a shop item data object.
     */
    void InitWithShopItem(const UPA_ShopItem* NewShopItem);

    /**
     * Helper to adapt a list of items to the tree node interface array.
     */
    TArray<const ITreeNodeInterface*> ItemsToInterfaces(const TArray<const UPA_ShopItem*>& Items) const;

    /**
     * The shop item associated with this widget.
     */
    UPROPERTY()
    const UPA_ShopItem* ShopItem;

    /**
     * The parent list view this entry belongs to (for selection management).
     */
    const class UListView* ParentListView;

    // Mouse handlers from base to trigger selection/purchase intents
    /**
     * Called when the right mouse button is clicked.
     */
    virtual void RightButtonClicked() override;

    /**
     * Called when the left mouse button is clicked.
     */
    virtual void LeftButtonClicked() override;
};
