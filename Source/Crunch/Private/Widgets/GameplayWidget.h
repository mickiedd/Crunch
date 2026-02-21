// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: GameplayWidget.h
// Purpose: Main in-game HUD. Displays crosshair, gauges, inventory and shop
//          toggles/overlays, and reacts to player/controller events.
// ----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GAS/CGameplayAbilityTypes.h"
#include "GameplayWidget.generated.h"

/**
 * Root gameplay HUD widget owned by `ACPlayerController`. Hosts or references
 * sub-widgets such as crosshair, stat gauges, and inventory/shop panels, and
 * exposes helper functions/events to update them.
 */
UCLASS()
class UGameplayWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    /**
     * Called when the widget is constructed.
     */
    virtual void NativeConstruct() override;

    /**
     * Called when the widget is destructed.
     */
    virtual void NativeDestruct() override;

    /**
     * Configures gameplay abilities.
     *
     * @param Abilities Map of ability input IDs to gameplay ability classes.
     */
    void ConfigureAbilities(const TMap<ECAbilityInputID, TSubclassOf<class UGameplayAbility>>& Abilities);

    /**
     * Toggles the shop widget.
     */
    void ToggleShop();

    /**
     * Toggles the gameplay menu.
     */
    UFUNCTION()
    void ToggleGameplayMenu();

    /**
     * Shows the gameplay menu.
     */
    void ShowGameplayMenu();

    /**
     * Sets the title of the gameplay menu.
     *
     * @param NewTitle New title for the gameplay menu.
     */
    void SetGameplayMenuTitle(const FString& NewTitle);

    /**
     * Event called when the shop state is updated.
     *
     * @param bVisible Whether the shop is visible.
     */
    UFUNCTION(BlueprintImplementableEvent)
    void UpdateShopState(bool bVisible);

    /**
     * Event called when the inventory state is updated.
     *
     * @param bVisible Whether the inventory is visible.
     */
    UFUNCTION(BlueprintImplementableEvent)
    void UpdateInventoryState(bool bVisible);

private:
    /**
     * Health bar widget.
     */
    UPROPERTY(meta=(BindWidget))
    class UValueGauge* HealthBar;

	UPROPERTY(meta=(BindWidget))
	class UValueGauge* ManaBar;

	UPROPERTY(meta=(BindWidget))
	class UAbilityListView* AbilityListView;

	UPROPERTY(meta=(BindWidget))
	class UStatsGauge* AttackDamageGauge;

	UPROPERTY(meta=(BindWidget))
	class UStatsGauge* ArmorGauge;

	UPROPERTY(meta=(BindWidget))
	class UStatsGauge* MoveSpeedGauge;

	UPROPERTY(meta=(BindWidget))
	class UStatsGauge* IntenlligenceGauge;

	UPROPERTY(meta=(BindWidget))
	class UStatsGauge* StrengthGauge;

	UPROPERTY(meta=(BindWidget))
	class UShopWidget* ShopWidget;

	UPROPERTY(meta=(BindWidget))
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(meta=(BindWidget))
	class USkeletalMeshRenderWidget* HeadshotWidget;

	UPROPERTY(meta=(BindWidget))
	class UMatchStatWidget* MatchStatWidget;

	UPROPERTY(meta=(BindWidget))
	class UGameplayMenu* GameplayMenu;

	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* MainSwitcher;

	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* GameplayWidgetRootPanel;

	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* GameplayMenuRootPanel;

	UPROPERTY(meta=(BindWidget))
	class UCrosshairWidget* CrosshairWidget;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	class UWidgetAnimation* ShopPopupAnimation;


	void PlayShopPopupAnimation(bool bPlayForward);
	void SetOwningPawnInputEnabled(bool bPawnInputEnabled);
	void SetShowMouseCursor(bool bShowMouseCursor);
	void SetFocusToGameAndUI();
	void SetFocusToGameOnly();

	UPROPERTY()
	class UAbilitySystemComponent* OwnerAbilitySystemComponent;
};
