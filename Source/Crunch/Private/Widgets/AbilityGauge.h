// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "AbilityGauge.generated.h"

class UAbilitySystemComponent;
struct FGameplayAbilitySpec;

/**
 * Data structure for configuring an ability gauge widget.
 */
USTRUCT(BlueprintType)
struct FAbilityWidgetData : public FTableRowBase
{
    GENERATED_BODY()

    /**
     * Ability class associated with this widget.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UGameplayAbility> AbilityClass;

    /**
     * Display name of the ability.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AbilityName;

    /**
     * Icon texture for the ability.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> Icon;

    /**
     * Description text for the ability.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
};

/**
 * Ability HUD cell implementing IUserObjectListEntry so it can be used inside
 * lists (e.g., `UListView`). It receives `FAbilityWidgetData` to configure its
 * visuals and listens to the owner's Ability System Component to reflect
 * cooldowns, mana requirements, learned level, and upgrade availability.
 */
UCLASS()
class UAbilityGauge : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()
public:
    /**
     * Native construct event.
     */
    virtual void NativeConstruct() override;

    /**
     * Called when the list item object is set.
     */
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

    /**
     * Configures the widget with the provided ability data.
     */
    void ConfigureWithWidgetData(const FAbilityWidgetData* WidgetData);

private:
    /**
     * Interval to tick cooldown text/material updates.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Cooldown")
    float CooldownUpdateInterval = 0.1f;

    /**
     * MID parameter names used by the gauge material.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    FName IconMaterialParamName = "Icon";

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    FName CooldownPercentParamname = "Percent";

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    FName AbilityLevelParamName = "Level";

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    FName CanCastAbilityParamName = "CanCast";
    
    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    FName UpgradePointAvaliableParamName = "UpgradeAvaliable";

    /**
     * Tooltip class used to show ability description and numbers.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Tool Tip")
    TSubclassOf<class UAbilityToolTip> AbilityToolTipClass;

    /**
     * Creates a tooltip widget for the ability.
     */
    void CreateToolTipWidget(const FAbilityWidgetData* AbilityWidgetData);

    /**
     * Icon image widget.
     */
    UPROPERTY(meta=(BindWidget))
    class UImage* Icon;

    /**
     * Level gauge image widget.
     */
    UPROPERTY(meta=(BindWidget))
    class UImage* LevelGauge;

    /**
     * Cooldown counter text widget.
     */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* CooldownCounterText;

    /**
     * Cooldown duration text widget.
     */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* CooldownDurationText;

    /**
     * Cost text widget.
     */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* CostText;

    /**
     * Cached ability CDO to query static data (icon, cost, tags).
     */
    UPROPERTY()
    class UGameplayAbility* AbilityCDO;

    /**
     * Called when the ability is committed.
     */
    void AbilityCommitted(UGameplayAbility* Ability);

    /**
     * Starts the cooldown timer.
     */
    void StartCooldown(float CooldownTimeRemaining, float CooldownDuration);

    float CachedCooldownDuration;
    float CachedCooldownTimeRemaining;

    FTimerHandle CooldownTimerHandle;
    FTimerHandle CooldownTimerUpdateHandle;

    FNumberFormattingOptions WholeNumberFormattionOptions;
    FNumberFormattingOptions TwoDigitNumberFormattingOptions;

    /**
     * Called when the cooldown finishes.
     */
    void CooldownFinished();

    /**
     * Updates the cooldown progress.
     */
    void UpdateCooldown();

    /**
     * Owning Ability System and spec handle for this entry.
     */
    const UAbilitySystemComponent* OwnerAbilitySystemComponent;
    FGameplayAbilitySpecHandle CachedAbilitySpecHandle;

    /**
     * Gets the ability spec.
     */
    const FGameplayAbilitySpec* GetAbilitySpec();

    bool bIsAbilityLeanred = false;

    /**
     * Called when the ability spec is updated.
     */
    void AbilitySpecUpdated(const FGameplayAbilitySpec& AbilitySpec);

    /**
     * Updates the can cast state.
     */
    void UpdateCanCast();

    /**
     * Called when the upgrade point is updated.
     */
    void UpgradePointUpdated(const FOnAttributeChangeData& Data);

    /**
     * Called when the mana is updated.
     */
    void ManaUpdated(const FOnAttributeChangeData& Data);

};
