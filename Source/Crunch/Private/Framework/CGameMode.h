// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GenericTeamAgentInterface.h"
// ----------------------------------------------------------------------------
// File: CGameMode.h
// Purpose: Core gameplay mode coordinating team spawns, default pawn selection,
//          and match finish flow. Chooses spawn points by team tag mapping and
//          can fall back to a backup pawn class if needed.
// Key API:
//  - SpawnPlayerController override for custom controller spawning.
//  - GetDefaultPawnClassForController / SpawnDefaultPawnFor to select pawns.
//  - TeamStartSpotTagMap: maps FGenericTeamId to PlayerStart tags for spawn.
//  - MatchFinished: notifies winning view/teams and triggers end-of-match.
// ----------------------------------------------------------------------------
#include "CGameMode.generated.h"

/**
 * GameModeBase subclass that assigns teams and handles team-based spawn spots.
 * Queries the map for tagged PlayerStarts per team via TeamStartSpotTagMap and
 * returns a suitable start. Integrates with AStormCore for win conditions and
 * uses BackupPawn as a fallback pawn class.
 */
UCLASS()
class ACGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACGameMode();
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, const FString& Options) override;
	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* Controller) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

private:
	FGenericTeamId GetTeamIDForPlayer(const AController* InController) const;

	AActor* FindNextStartSpotForTeam(const FGenericTeamId& TeamID) const;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	TSubclassOf<APawn> BackupPawn;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	TMap<FGenericTeamId, FName> TeamStartSpotTagMap;

	class AStormCore* GetStormCore() const;

	void MatchFinished(AActor* ViewTarget, int WiningTeam);
};
