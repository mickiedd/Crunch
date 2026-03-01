// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BehaviacAnimalBase.h"
#include "BehaviacTypes.h"

// Sets default values
ABehaviacAnimalBase::ABehaviacAnimalBase()
{
	// Create Behaviac agent component
	BehaviacAgent = CreateDefaultSubobject<UBehaviacAgentComponent>(TEXT("BehaviacAgent"));
	// Manually tick from our own Tick() for ordering control
	BehaviacAgent->bAutoTick = false;

}

// Called when the game starts or when spawned
void ABehaviacAnimalBase::BeginPlay()
{
	Super::BeginPlay();

	if (!BehaviacAgent)
	{
		BehaviacAgent = FindComponentByClass<UBehaviacAgentComponent>();
		if (BehaviacAgent)
		{
			BEHAVIAC_VLOG(TEXT("[BehaviacPenguin] %s %p: BehaviacAgent pointer recovered via FindComponentByClass (Blueprint CDO deserialization had nulled it)"), *GetName(), this);
		}
		else
		{
			UE_LOG(LogBehaviac, Error, TEXT("[BehaviacPenguin] %s %p: BehaviacAgent component truly missing — cannot run behavior tree"), *GetName(), this);
			return;
		}
	}

	// ── Load behavior tree ─────────────────────────────────────────────
	bool bLoaded = false;
	if (BehaviacAgent)
	{
		if (BehaviorTree)
		{
			bLoaded = BehaviacAgent->LoadBehaviorTree(BehaviorTree);
			BEHAVIAC_VLOG(TEXT("[Behaviac] %s: behavior tree loaded from asset reference"), *GetName());
		}
		else if (!BehaviorTreeAssetPath.IsEmpty())
		{
			BehaviacAgent->LoadBehaviorTreeByPath(BehaviorTreeAssetPath);
			BEHAVIAC_VLOG(TEXT("[Behaviac] %s: behavior tree loaded from asset path"), *GetName());
			bLoaded = true; // path-based load doesn't return bool
		}
	}
	if (bLoaded)
	{
		BEHAVIAC_VLOG(TEXT("[Behaviac] %s %p: behavior tree loaded OK"), *GetName(), this);
	}
	else
	{
		UE_LOG(LogBehaviac, Error, TEXT("[Behaviac] %s %p: failed to load behavior tree!"), *GetName(), this);
		// Early return to avoid null dereference if tree is missing
		return;
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

