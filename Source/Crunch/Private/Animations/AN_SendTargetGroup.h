// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
// ----------------------------------------------------------------------------
// File: AN_SendTargetGroup.h
// Purpose: Animation notify that finds a group of actors near specified sockets
//          on the owner mesh and sends local gameplay cues/events to them. Used
//          to implement melee/area effects aligned to montage windows.
// Key API:
//  - TriggerGameplayCueTags: cue tags to trigger on each impacted target.
//  - TargetTeam: filter targets by ETeamAttitude (e.g., Hostile/Neutral/Friendly).
//  - SphereSweepRadius: radius to search around each socket.
//  - bDrawDebug/bIgnoreOwner: debug and self-filtering options.
//  - EventTag: optional gameplay event tag to dispatch.
//  - TargetSocketNames: sockets on the mesh to center the sweeps.
// Integration:
//  - Notify: performs the sweep(s), filters by team attitude, and calls
//    SendLocalGameplayCue to actually emit the cue/event per hit.
// ----------------------------------------------------------------------------
#include "AN_SendTargetGroup.generated.h"

/**
 * UAnimNotify that performs sphere sweeps at one or more sockets to collect
 * potential targets and send gameplay cues/events to each.
 *
 * Behavior:
 *  - On Notify, for each TargetSocketName, do a sphere sweep with radius
 *    SphereSweepRadius around the socket transform.
 *  - Evaluate attitude (TargetTeam) against the owner's team via
 *    IGenericTeamAgentInterface and filter results.
 *  - For each valid hit, optionally draw debug, then trigger local gameplay
 *    cues and/or a gameplay event tagged by EventTag.
 *
 * Notes:
 *  - Typical use: align hit detection and cue triggering to montage frames.
 *  - bIgnoreOwner avoids self-hits during close-range sweeps.
 */
UCLASS()
class UAN_SendTargetGroup : public UAnimNotify
{
	GENERATED_BODY()
public:	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	FGameplayTagContainer TriggerGameplayCueTags;

	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	TEnumAsByte<ETeamAttitude::Type> TargetTeam = ETeamAttitude::Hostile;

	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	float SphereSweepRadius = 60.f;

	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	bool bDrawDebug = true;

	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	bool bIgnoreOwner = true;

	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	FGameplayTag EventTag;
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	TArray<FName> TargetSocketNames;

	void SendLocalGameplayCue(const FHitResult& HitResult) const;
};
