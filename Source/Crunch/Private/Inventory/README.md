# Inventory Subsystem Documentation

Updated: 2025-08-14

This document explains the inventory system under `Source/Crunch/Private/Inventory`. It covers item/data assets, runtime item objects, the inventory component, and their integrations with GAS and the AssetManager.

Contents
- UPA_ShopItem (Primary Data Asset)
- FItemCollection (Helper type)
- UInventoryItem (Runtime item object)
- UInventoryComponent (Inventory manager component)
- Integration Points
- Typical Flows
- Extension Notes

## UPA_ShopItem
Files: `PA_ShopItem.h/.cpp`

- Purpose: Defines a shop item with pricing, display info, and gameplay hooks.
- Responsibilities
  - Grant a Gameplay Ability and/or apply Gameplay Effects when equipped/consumed.
  - Define consumable/stackable semantics (max stack), and crafting ingredients for combination.
- Key Methods
  - Primary Asset: `GetPrimaryAssetId()`, `static GetShopItemAssetType()`.
  - Accessors: `GetIcon()`, `GetItemName()`, `GetItemDescription()`, `GetPrice()`, `GetSellPrice()`.
  - Gameplay hooks: `GetEquippedEffect()`, `GetConsumeEffect()`, `GetGrantedAbility()`, `GetGrantedAbilityCDO()`.
  - Recipe: `GetIngredients()` returns soft references to ingredient items.
- Properties
  - `Icon`, `ItemName`, `ItemDescription`, `Price`, `bIsConsumable`.
  - Gameplay: `EquippedEffect`, `ConsumeEffect`, `GrantedAbility`.
  - Stack: `bIsStackable`, `MaxStackCount`.
  - Crafting: `IngredientItems`.

## FItemCollection
Files: `PA_ShopItem.h/.cpp`

- Purpose: Lightweight wrapper collection used to build mapping graphs (combinations, ingredients).
- Key Methods: `AddItem`, `Contains`, `GetItems`.

## UInventoryItem
Files: `InventoryItem.h/.cpp`

- Purpose: Runtime object for an owned inventory entry bound to a `UPA_ShopItem`.
- Responsibilities
  - Track stack count, slot assignment, and validity.
  - Apply/remove equipped effects; grant an ability and activate it; apply consume effect.
  - Expose cooldown, duration, and mana cost queries for UI; signal `OnAbilityCanCastUpdated`.
- Key Methods
  - Initialization: `InitItem(...)`, `IsValid()`.
  - Stack: `AddStackCount()`, `ReduceStackCount()`, `SetStackCount(int)`, `IsStackFull()`.
  - Identity: `GetHandle()`, `GetShopItem()`.
  - Activation/Effects: `TryActivateGrantedAbility()`, `ApplyConsumeEffect()`, `RemoveGASModifications()`.
  - UI Queries: `GetAbilityCooldownTimeRemaining()`, `GetAbilityCooldownDuration()`, `GetAbilityManaCost()`, `CanCastAbility()`.
  - Slot: `SetSlot(int)`, `GetItemSlot()`.
- Properties
  - `Handle` (FInventoryItemHandle), `ShopItem`, `StackCount`, `Slot`.
  - GAS: `AppliedEquipedEffectHandle`, `GrantedAbiltiySpecHandle`, and internal `OwnerAbilitySystemComponent`.

### FInventoryItemHandle
- Purpose: Opaque, unique identifier for inventory items that is safe to replicate and use in maps/sets.
- Methods: `CreateHandle()`, `InvalidHandle()`, `IsValid()`, `GetHandleId()`.

## UInventoryComponent
Files: `InventoryComponent.h/.cpp`

- Purpose: Actor component that manages items for a character/pawn, including authoritative server actions for purchase, activation, and selling.
- Responsibilities
  - Maintain a map from `FInventoryItemHandle` to `UInventoryItem*` and expose capacity rules.
  - Handle stacking, slot changes, and item combination recipes using `UCAssetManager` maps.
  - Integrate with GAS to activate item abilities and broadcast cooldown info.
  - Replicate actions via RPCs and update client UI via delegates.
- Key Methods
  - Public: `TryPurchase(...)`, `TryActivateItem(...)`, `SellItem(...)`, `GetGold()`, `GetCapacity()`.
  - Query/Utility: `IsFullFor(...)`, `IsAllSlotOccupied()`, `GetAvaliableStackForItem(...)`, `FindIngredientForItem(...)`, `TryGetItemForShopItem(...)`, `TryActivateItemInSlot(...)`.
  - Slot change callback: `ItemSlotChanged(...)`.
  - Server RPCs: `Server_Purchase(...)`, `Server_ActivateItem(...)`, `Server_SellItem(...)`.
  - Client RPCs: `Client_ItemAdded(...)`, `Client_ItemRemoved(...)`, `Client_ItemStackCountChanged(...)`.
  - Internal: `GrantItem(...)`, `ConsumeItem(...)`, `RemoveItem(...)`, `TryItemCombination(...)`, `AbilityCommitted(...)`.
- Properties
  - `Capacity`, `InventoryMap`, `OwnerAbilitySystemComponent`.
- Delegates
  - `OnItemAdded`, `OnItemRemoved`, `OnItemStackCountChanged`, `OnItemAbilityCommitted`.

## Integration Points
- With UCAssetManager
  - Uses combination/ingredient maps from `UCAssetManager` to resolve crafting results and ingredient searches.
- With GAS
  - Applies `UPA_ShopItem` effects, grants abilities, checks cooldowns/mana, and fires `OnItemAbilityCommitted` with cooldown data.
- With UI
  - Delegates provide push updates for inventory widgets and hotbar indicators; slot changes notify UI for reordering.

## Typical Flows
1. Purchase: Client calls `TryPurchase` → Server validates → `GrantItem` → Client `Client_ItemAdded` with handle/spec.
2. Activate: Client calls `TryActivateItem` → Server validates and activates ability or consumes item → broadcasts `OnItemAbilityCommitted`.
3. Combine: When acquiring an item, server checks `TryItemCombination` using AssetManager combination maps; replaces ingredients with resulting item.
4. Sell: Client calls `SellItem` → Server removes item → Client `Client_ItemRemoved`.

## Extension Notes
- Add rarity tiers and sort/group rules for UI.
- Extend combination rules (e.g., multi-step recipes, partial refunds).
- Introduce equipment slots and paper-doll visuals, with persistent save data.
- Add analytics hooks for purchase/usage rates.
