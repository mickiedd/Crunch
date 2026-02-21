// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: MatchStatWidget.h
// Purpose: Displays match objective progress and team counts, subscribing to
//          StormCore updates and animating a progress bar via a MID param.
// ----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchStatWidget.generated.h"

/**
 * HUD element showing team influence and match completion progress. Uses a
 * timer to periodically update a dynamic material parameter on ProgressImage.
 */
UCLASS()
class UMatchStatWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
private:
	/** Seconds between progress updates for smoother animation */
	UPROPERTY(EditDefaultsOnly, Category = "Match Stat")
	float ProgressUpdateInterval = 0.5f;

	/** Material parameter name used to drive progress amount [0..1] */
	UPROPERTY(EditDefaultsOnly, Category = "Match Stat")
	FName ProgressDynamicMaterialParamName = "Progress";

	UPROPERTY(meta=(BindWidget))
	class UImage* ProgressImage;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TeamOneCountText;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TeamTwoCountText;

	UPROPERTY()
	class AStormCore* StormCore;

	/** Update team count texts */
	void UpdateTeamInfluence(int TeamOneCount, int TeamTwoCount);

	/** Called when match is finished to update final state */
	void MatchFinished(AActor* ViewTarget, int WinningTeam);
	/** Recompute and apply progress to the material */
	void UpdateProgress();

	FTimerHandle UpdateProgressTimerHandle;
};
