// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviacAnimalBase.h"

// Sets default values
ABehaviacAnimalBase::ABehaviacAnimalBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABehaviacAnimalBase::BeginPlay()
{
	Super::BeginPlay();

	// ── Load behavior tree ─────────────────────────────────────────────
	bool bLoaded = false;
	if (BehaviorTree)
	{
		bLoaded = BehaviacAgent->LoadBehaviorTree(BehaviorTree);
	}
	else if (!BehaviorTreeAssetPath.IsEmpty())
	{
		BehaviacAgent->LoadBehaviorTreeByPath(BehaviorTreeAssetPath);
		bLoaded = true; // path-based load doesn't return bool
	}

	if (bLoaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Behaviac] %s: behavior tree loaded OK"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[Behaviac] %s: failed to load behavior tree!"), *GetName());
	}
}

// Called every frame
void ABehaviacAnimalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABehaviacAnimalBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

