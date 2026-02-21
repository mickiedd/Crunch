// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"
// ----------------------------------------------------------------------------
// File: MinionBarrack.h
// Purpose: Defines AMinionBarrack, a server-side spawner and simple pool
//          manager for AMinion units. Spawns groups on an interval, assigns
//          team and goal, and reuses inactive minions when available.
// Key API:
//  - BarrackTeamId, MinionPerGroup, GroupSpawnInterval: spawn configuration.
//  - Goal, MinionClass, SpawnSpots: runtime wiring for behavior and placement.
//  - SpawnNewGroup / SpawnNewMinions / GetNextAvaliableMinion / GetNextSpawnSpot
//    implement pooling and round-robin spawn spot selection.
// ----------------------------------------------------------------------------
#include "MinionBarrack.generated.h"

/**
 * AMinionBarrack is a server-side spawner and simple pool manager for AMinion units.
 * It spawns groups of minions on a configurable interval, assigns them to a team and goal,
 * and reuses inactive minions when available.
 */
UCLASS()
class AMinionBarrack : public AActor
{
    GENERATED_BODY()
    
public: 
    // Sets default values for this actor's properties
    AMinionBarrack();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public: 
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere, Category = "Spawn")
    FGenericTeamId BarrackTeamId;
    
    UPROPERTY(EditAnywhere, Category = "Spawn")
    int MinionPerGroup = 3;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    float GroupSpawnInterval = 5.f;
    
    UPROPERTY()
    TArray<class AMinion*> MinionPool;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    AActor* Goal;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<class AMinion> MinionClass;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TArray<class APlayerStart*> SpawnSpots;

    int NextSpawnSpotIndex = -1;

    const APlayerStart* GetNextSpawnSpot();

    void SpawnNewGroup();
    void SpawnNewMinions(int Amt);
    AMinion* GetNextAvaliableMinion() const;

    FTimerHandle SpawnIntervalTimerHandle;

};
