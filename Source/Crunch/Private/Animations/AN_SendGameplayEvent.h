// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
// ----------------------------------------------------------------------------
// File: AN_SendGameplayEvent.h
// Purpose: Animation notify that fires a Gameplay Event (via GameplayTag)
//          when hit during animation playback. Typically used to sync GAS
//          ability events with montage frames.
// Key API:
//  - EventTag: the gameplay event tag to send when Notify triggers.
//  - Notify(...): called by animation system to perform the send.
// ----------------------------------------------------------------------------
#include "AN_SendGameplayEvent.generated.h"

/**
 * UAnimNotify that sends a GAS gameplay event tagged by EventTag when reached.
 *
 * Behavior:
 *  - Notify is invoked on montage/sequence playback at the notify frame.
 *  - Implementation retrieves the appropriate ASC/owner and dispatches a
 *    gameplay event with EventTag so subscribed abilities/effects can react.
 *
 * Notes:
 *  - Use for precise timing of ability logic (e.g., damage windows, VFX cues).
 *  - GetNotifyName_Implementation returns a friendly editor label.
 */
UCLASS()
class UAN_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
private:
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	FGameplayTag EventTag;
	virtual FString GetNotifyName_Implementation() const; 
};
