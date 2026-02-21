// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GAS/CGameplayAbilityTypes.h"
// ----------------------------------------------------------------------------
// File: BTTask_SendInputToAbilitySystem.h
// Purpose: Behavior Tree task that bridges AI behavior to the Gameplay Ability
//          System (GAS) by programmatically "pressing" an ability input ID on
//          the controlled pawn's Ability System Component (ASC).
// Key API:
//  - Property: ECAbilityInputID InputID (editable) selects which ability input
//              to press when this task runs.
//  - ExecuteTask: Resolves AAIController -> Pawn -> ASC and calls PressInputID.
// Integration:
//  - Used from Behavior Trees to trigger abilities the same way player input
//    would, keeping AI and player activation paths consistent.
// ----------------------------------------------------------------------------
#include "BTTask_SendInputToAbilitySystem.generated.h"

/**
 * Behavior Tree task that triggers a GAS ability input on the controlled pawn.
 *
 * Execution:
 *  - On ExecuteTask, find the owning AAIController and its pawn.
 *  - Retrieve the pawn's Ability System Component via
 *    UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent.
 *  - Call PressInputID((int32)InputID) to simulate a pressed input for GAS.
 *  - Returns Succeeded on success, otherwise Failed.
 *
 * Notes:
 *  - InputID maps to ECAbilityInputID which the project uses to bind abilities.
 *  - This enables BT-authored behaviors to activate abilities like players do.
 */
UCLASS()
class UBTTask_SendInputToAbilitySystem : public UBTTaskNode
{
    GENERATED_BODY()
public: 
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
    UPROPERTY(EditAnywhere, Category = "Ability")
    ECAbilityInputID InputID;
};
