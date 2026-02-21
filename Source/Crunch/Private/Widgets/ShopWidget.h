// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// ----------------------------------------------------------------------------
// File: ShopWidget.h
// Purpose: Shop UI that lists purchasable items and shows combination (recipe)
//          trees. Forwards buy/combine requests to the owner's inventory.
// ----------------------------------------------------------------------------
#include "ShopWidget.generated.h"

class UPA_ShopItem;
class UTileView;
class UInventoryComponent;
class UShopItemWidget;
/**
 * Displays shop items in a tile view and a combination tree for the selected
 * item. Caches generated item widgets and holds a pointer to the owning
 * `UInventoryComponent` to perform purchases.
 */
UCLASS()
class UShopWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
private:
    UPROPERTY(meta=(BindWidget))
    UTileView* ShopItemList;

    UPROPERTY(meta=(BindWidget))
    class UItemTreeWidget* CombinationTree;

    void LoadShopItems();
    void ShopItemLoadFinished();
    void ShopItemWidgetGenerated(UUserWidget& NewWidget);

    UPROPERTY()
    TMap<const UPA_ShopItem*, const UShopItemWidget*> ItemsMap;

    UPROPERTY()
    UInventoryComponent* OwnerInventoryComponent;

    void ShowItemCombination(const UShopItemWidget* ItemWidget);
};
