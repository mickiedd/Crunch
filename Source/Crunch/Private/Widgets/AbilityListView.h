// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "GAS/CGameplayAbilityTypes.h"
// ----------------------------------------------------------------------------
// File: AbilityListView.h
// Purpose: ListView that displays player abilities using UAbilityGauge entries.
//          It maps input IDs to ability classes, looks up row data, and
//          configures each generated entry widget.
// ----------------------------------------------------------------------------
#include "AbilityListView.generated.h"

/**
 * Populates a list of ability UI entries from an inputID->AbilityClass map.
 * Each row is configured with data from a UDataTable of FAbilityWidgetData.
 */
UCLASS()
class UAbilityListView : public UListView
{
    GENERATED_BODY()
public:
    /** Provide abilities to display; will generate/refresh list entries */
    void ConfigureAbilities(const TMap<ECAbilityInputID, TSubclassOf<class UGameplayAbility>>& Abilities);

private:
    /** Data table containing FAbilityWidgetData rows keyed by ability name */
    UPROPERTY(EditAnywhere, Category = "Data")
    UDataTable* AbilityDataTable;

    /** Called for each generated row widget to finish configuration */
    void AbilityGaugeGenerated(UUserWidget& Widget);

    /** Finds FAbilityWidgetData for the provided ability class, if any */
    const struct FAbilityWidgetData* FindWidgetDataForAbility(const TSubclassOf<UGameplayAbility>& AbilityClass) const;
};
