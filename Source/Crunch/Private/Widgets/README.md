# Widgets Subsystem

UI widgets for gameplay HUD, inventory/shop, lobby, and main menu. These are UMG-driven `UUserWidget` classes (plus supporting UI-specific actors) that compose the user interface and interact with Player, Inventory, and Framework subsystems.

Contents
- Gameplay HUD
  - GameplayWidget: Root in-game HUD that hosts gauges, crosshair, inventory and shop overlays.
  - CrosshairWidget: Displays and updates the weapon crosshair/reticle.
  - ValueGauge / LevelGauge / StatsGauge / AbilityGauge / OverHeadStatsGauge: Bars and aggregates for health, mana, XP/level, ability cooldown/states, overhead stat display.
- Inventory & Shop
  - InventoryWidget: Inventory panel with drag/drop and context menu.
  - InventoryItemWidget: Individual item cell widget; drag source/target logic.
  - InventoryContextMenuWidget: Context menu for item actions (use, drop, split, sell).
  - InventoryItemDragDropOp: Drag-drop operation payload for inventory items.
  - ItemWidget: Generic item display (icon/name/count) used across lists.
  - ItemToolTip / AbilityToolTip: Tooltip widgets for items and abilities.
  - ItemTreeWidget / TreeNodeInterface: Tree-based view for compound items/recipes.
  - ShopWidget / ShopItemWidget: Shop UI for listing, filtering, and purchasing items.
- Menus (Lobby/Main Menu)
  - MainMenuWidget: Landing UI for hosting/joining sessions.
  - SessionEntryWidget: One session entry row in the browser.
  - LobbyWidget: Team slots, hero selection, and ready-up controls.
  - TeamSelectionWidget, PlayerTeamLayoutWidget, PlayerTeamSlotWidget: Lobby layout building blocks.
  - WaitingWidget: Interstitial waiting/progress UI.
- 3D UI Helpers
  - RenderActor / SkeletalMeshRenderActor: Lightweight actors used to render static/skeletal meshes for UI thumbnails.
  - RenderActorTargetInterface: Interface for widgets that own/render such actors.
  - RenderActorWidget / SkeletalMeshRenderWidget / CharacterDisplay: Widgets that spawn and manage render actors.
- Misc
  - AbilityListView: Displays abilities and binds input IDs.
  - CharacterEntryWidget: Entry in character selection lists.
  - GameplayMenu: Pause/gameplay menu overlay.
  - SplineWidget: Utility for drawing spline-based UI (paths, connections).

Key Integrations
- Inventory subsystem: inventory item structs and component; drag-drop and context menu actions route to `UInventoryComponent` on the owning character.
- Player subsystem: `UGameplayWidget` is owned by `ACPlayerController` and mirrors controller-driven UI toggles.
- GAS: HUD gauges and ability lists reflect attribute and ability state; widgets may call Blueprint events with `ECAbilityInputID`.
- Online/Network: Session and lobby widgets use `ACPlayerState` and OSS-driven data to populate lists and team slots.

Typical Flows
- In-Game HUD
  1) `ACPlayerController` spawns `UGameplayWidget` on possession.
  2) HUD binds to character attribute changes and ability inputs.
  3) Shop/Inventory toggles call Blueprint events to show/hide panels.
- Inventory Drag & Drop
  1) Begin drag from `InventoryItemWidget` -> `InventoryItemDragDropOp` carries item handle and source slot.
  2) Drop over another cell or context menu triggers operations on `UInventoryComponent` (move/combine/use).
- Lobby
  1) `LobbyWidget` displays `FPlayerSelection` data; team slot widgets allow switching.
  2) Hero selection lists use `CharacterEntryWidget`; selection is committed via `ACPlayerState` server RPC.

Extension Notes
- Prefer binding to attribute sets and ability system delegates in `NativeConstruct` and unbind in `NativeDestruct`.
- Keep drag-drop payloads lightweight (`FInventoryItemHandle`/ids) and validate on server before acting.
- Expose pure BlueprintImplementableEvents for UI skinning and animations; keep authoritative logic in C++.

Key Classes and Responsibilities
- GameplayWidget
  - Owns crosshair, gauges, ability list, inventory and shop overlays, gameplay menu, and match stats.
  - Bridges controller input to UI visibility (ToggleShop, ToggleGameplayMenu) and focuses input between Game and UI.
- AbilityListView
  - Accepts a map of `ECAbilityInputID -> UGameplayAbility` and configures spawned entry widgets (e.g., AbilityGauge) using `FAbilityWidgetData` rows from a `UDataTable`.
- InventoryWidget / InventoryItemWidget
  - Grid/list of item cells. Each cell supports drag source/target, context menu open, and tooltip display.
- ShopWidget / ShopItemWidget
  - Presents shop inventory and emits selection/purchase intents. Shop items also implement `ITreeNodeInterface` to visualize combination trees via `UItemTreeWidget`.
- ItemTreeWidget
  - Lays out upstream (components) and downstream (upgrades) nodes and draws connections between them on a canvas.
- WaitingWidget / GameplayMenu / MainMenuWidget / LobbyWidget family
  - Small, focused widgets for specific overlays and flows (pause, waiting/cancel, lobby team selection, session browser).

Delegate and Event Map (selected)
- ShopItemWidget
  - OnItemPurchaseIssued(const UPA_ShopItem*): broadcast when purchase confirmed.
  - OnShopItemClicked(const UShopItemWidget*): broadcast when list entry is selected.
- SessionEntryWidget
  - OnSessionEntrySelected(const FString&): broadcast selected session ID.
- TeamSelectionWidget
  - OnSlotClicked(uint8): broadcast selected team slot.
- GameplayWidget
  - UpdateShopState(bool bVisible) [BlueprintImplementableEvent]
  - UpdateInventoryState(bool bVisible) [BlueprintImplementableEvent]

Data Flow Details
- Inventory Drag/Drop
  - Source: InventoryItemWidget creates `UInventoryItemDragDropOp` in `NativeOnDragDetected`, sets source widget and optional drag visual (derived from `UItemWidget`).
  - Target: Destination InventoryItemWidget or panel inspects the operation payload to determine source slot/item and calls owning `UInventoryComponent` to move/swap/split/use.
  - Context Menu: Right-click opens `UInventoryContextMenuWidget`; its button events are bound by InventoryWidget to trigger actions.
- Shop Purchase
  - Selection: Clicking a `UShopItemWidget` emits `OnShopItemClicked` and updates side panels (tooltip, recipe tree, details).
  - Purchase: Confirming purchase emits `OnItemPurchaseIssued` with the `UPA_ShopItem*`; ShopWidget forwards to server-side purchase logic (e.g., via PlayerController/InventoryComponent).

GAS Binding Summary
- Ability display uses `AbilityListView` to create `AbilityGauge` widgets that:
  - Show icon, input binding, level, mana cost, and cooldown overlay.
  - Bind to `UAbilitySystemComponent` delegates to react to ability activation, cooldown tag changes, and attribute updates affecting cost/availability.
- ValueGauge/StatsGauge widgets subscribe to AttributeSets (Health, Mana, AD, Armor, MoveSpeed, etc.) and update progress bars and text.

3D UI Helpers
- RenderActorWidget / SkeletalMeshRenderWidget spawn lightweight actors (e.g., `URenderActor`/`USkeletalMeshRenderActor`) to render meshes in UI thumbnails.
- Use `RenderActorTargetInterface` for loose coupling so a widget owns and poses the preview actor without gameplay-side dependencies.
