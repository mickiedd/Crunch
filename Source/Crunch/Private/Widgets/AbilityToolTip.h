// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: AbilityToolTip.h
// Purpose: Tooltip widget for abilities showing name, icon, description,
//          cooldown, and cost. Populated by AbilityGauge or similar widgets.
// ----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityToolTip.generated.h"

/**
 * Lightweight tooltip for displaying ability metadata. Call SetAbilityInfo to
 * update bound UI elements when the hovered ability changes.
 */
UCLASS()
class UAbilityToolTip : public UUserWidget
{
    GENERATED_BODY()
public:
    /** Sets all tooltip display fields for the given ability */
    void SetAbilityInfo(const FName& AbilityName, UTexture2D* AbilityTexture, const FText& AbilityDescription, float AbilityCooldown, float AbilityCost);

private: 
    /** Ability display name */
    UPROPERTY(meta=(BindWidget)) 
    class UTextBlock* AbilityNameText;

    /** Ability icon image */
    UPROPERTY(meta=(BindWidget)) 
    class UImage* AbilityIcon;

    /** Localized description text */
    UPROPERTY(meta=(BindWidget)) 
    class UTextBlock* AbilityDescriptionText;

    /** Cooldown text (e.g., seconds) */
    UPROPERTY(meta=(BindWidget)) 
    class UTextBlock* AbilityCooldownText;

    /** Ability cost text */
    UPROPERTY(meta=(BindWidget)) 
    class UTextBlock* AbilityCostText;
};
