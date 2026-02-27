// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviacDog.h"

// Sets default values
ABehaviacDog::ABehaviacDog()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set a safe default idle tree (prevents crash if not set in editor)
	BehaviorTreeAssetPath = TEXT("AI/BehaviacTrees/DogIdleTree");
}

// Called when the game starts or when spawned
void ABehaviacDog::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABehaviacDog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

