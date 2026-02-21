// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// ----------------------------------------------------------------------------
// File: ItemWidget.h
// Purpose: Base widget for displaying an item icon (and for deriving
//          interactive item cells like InventoryItemWidget/ShopItemWidget).
// ----------------------------------------------------------------------------
#include "ItemWidget.generated.h"


class UItemToolTip;
class UPA_ShopItem;
/**
 * Base item UI widget that owns an icon image, optional tooltip, and common
 * mouse handling that routes left/right clicks to virtual handlers.
 */
UCLASS()
class UItemWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    /** Assigns/updates the item icon texture (creates/updates MID if needed) */
    virtual void SetIcon(UTexture2D* IconTexture);
protected:
    /** Create and assign a tooltip widget for the given shop/inventory item */
    UItemToolTip* SetToolTipWidget(const UPA_ShopItem* Item);
    /** Accessor for derived classes to manipulate the bound icon image */
    class UImage* GetItemIcon() const { return ItemIcon; }

private:
    UPROPERTY(meta=(BindWidget))
    class UImage* ItemIcon;

    UPROPERTY(EditDefaultsOnly, Category = "ToolTip")
    TSubclassOf<UItemToolTip> ItemToolTipClass;

    virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
    virtual FReply NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

    /** Override points for derived widgets to react to mouse clicks */
    virtual void RightButtonClicked();
    virtual void LeftButtonClicked();
};
