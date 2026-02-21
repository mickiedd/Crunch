// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
// ----------------------------------------------------------------------------
// File: CrosshairWidget.h
// Purpose: HUD reticle widget that tracks aim target and changes color based on
//          target presence. Also re-centers to screen aim location each tick.
// ----------------------------------------------------------------------------
#include "CrosshairWidget.generated.h"

/**
 * Simple crosshair HUD element. Listens for gameplay tag/count updates and
 * gameplay events to determine if the player currently has an aim target, and
 * updates color/position accordingly.
 */
UCLASS()
class UCrosshairWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
private:
    /** Color when aim has a valid target */
    UPROPERTY(EditDefaultsOnly, Category = "View")
    FLinearColor HasTargetColor = FLinearColor::Red;

    /** Color when there is no valid target */
    UPROPERTY(EditDefaultsOnly, Category = "View")
    FLinearColor NoTargetColor = FLinearColor::White;

    UPROPERTY(meta=(BindWidget))
    class UImage* CrosshairImage;

    /** Callback for gameplay tag stack updates that affect crosshair state */
    void CrosshairTagUpdated(const FGameplayTag Tag, int32 NewCount);

    UPROPERTY()
    class UCanvasPanelSlot* CrosshairCanvasPanelSlot;

    UPROPERTY()
    class APlayerController* CachedPlayerController;

    /** Positions crosshair to the current screen center or aim location */
    void UpdateCrosshairPosition();

    UPROPERTY()
    const AActor* AimTarget;

    /** Gameplay event hook to update current aim target */
    void TargetUpdated(const struct FGameplayEventData* EventData);
};
