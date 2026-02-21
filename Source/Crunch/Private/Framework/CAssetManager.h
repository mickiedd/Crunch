// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Inventory/PA_ShopItem.h"
// ----------------------------------------------------------------------------
// File: CAssetManager.h
// Purpose: Centralized asset manager for async loading and lightweight runtime
//          asset queries. Handles character definitions and shop items, and
//          builds item combination/ingredient maps for quick lookup.
// Key API:
//  - LoadCharacterDefinations / GetLoadedCharacterDefinations
//  - LoadShopItems / GetLoadedShopItems
//  - GetCombinationForItem / GetIngredientForItem (built from shop items)
// ----------------------------------------------------------------------------
#include "CAssetManager.generated.h"

class UPA_CharacterDefination;
/**
 * UAssetManager subclass that exposes convenience loaders and lookups for
 * character definitions and shop items. Internally uses streamable manager to
 * async load assets and constructs combination/ingredient maps on completion.
 */
UCLASS()
class UCAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UCAssetManager& Get();
	void LoadCharacterDefinations(const FStreamableDelegate& LoadFinishedCallback);
	bool GetLoadedCharacterDefinations(TArray<UPA_CharacterDefination*>& LoadedCharacterDefinations) const;

	void LoadShopItems(const FStreamableDelegate& LoadFinishedCallback);
	bool GetLoadedShopItems(TArray<const UPA_ShopItem*>& OutItems) const;
	const FItemCollection* GetCombinationForItem(const UPA_ShopItem* Item) const;
	const FItemCollection* GetIngredientForItem(const UPA_ShopItem* Item) const;
private:
	void ShopItemLoadFinished(FStreamableDelegate Callback);
	void BuildItemMaps();
	void AddToCombinationMap(const UPA_ShopItem* Ingredient, const UPA_ShopItem* CombinationItem);

	UPROPERTY()
	TMap<const UPA_ShopItem*, FItemCollection> CombinationMap;
	UPROPERTY()
	TMap<const UPA_ShopItem*, FItemCollection> IngredientMap;
};
