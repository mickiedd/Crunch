// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// ----------------------------------------------------------------------------
// File: ItemToolTip.h
// Purpose: Tooltip for shop/inventory items. Shows icon, title, description,
//          and dynamic price. Consumed by ItemWidget/ShopItemWidget.
// ----------------------------------------------------------------------------
#include "ItemToolTip.generated.h"

class UPA_ShopItem;
/**
 * Displays metadata for an item and supports updating the price at runtime
 * (e.g., discounts, sell value). Use SetItem and SetPrice to populate fields.
 */
UCLASS()
class UItemToolTip : public UUserWidget
{
    GENERATED_BODY()
    
public:
    /** Populate tooltip using shop item data (icon, title, description, price) */
    void SetItem(const UPA_ShopItem* Item);
    /** Update price text only (without re-supplying the item) */
    void SetPrice(float newPrice);
private:
    /** Item icon shown on the tooltip */
    UPROPERTY(meta=(BindWidget))
    class UImage* IconImage;

    /** Localized item name/title */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* ItemTitleText;

    /** Localized description details */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* ItemDescriptionText;

    /** Numeric price (buy/sell) */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* ItemPriceText;
};
