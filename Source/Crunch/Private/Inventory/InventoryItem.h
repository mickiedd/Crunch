// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: InventoryItem.h
// Purpose: Defines the logical inventory item object and its lightweight
//          handle type. Items encapsulate stack counts, slot assignment, and
//          integration points with GAS (effects, granted ability, cooldowns
//          and mana cost checks).
// ----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "InventoryItem.generated.h"

class UPA_ShopItem;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityCanCastUpdatedDelegate, bool /*bCanCast*/)

/**
 * Unique, opaque identifier for an inventory item instance. Safe to use
 * as a TMap/TSet key (hash provided below) and to reference items across
 * client/server RPCs without exposing object pointers.
 */
USTRUCT()
struct FInventoryItemHandle
{
    GENERATED_BODY()
public:
    FInventoryItemHandle();
    static FInventoryItemHandle InvalidHandle();
    static FInventoryItemHandle CreateHandle();

    bool IsValid() const;
    uint32 GetHandleId() const { return HandleId; }
private:
    explicit FInventoryItemHandle(uint32 Id);

    UPROPERTY()
    uint32 HandleId;

    static uint32 GenerateNextId();
    static uint32 GetInvalidId();
};

bool operator==(const FInventoryItemHandle& Lhs, const FInventoryItemHandle& Rhs);
uint32 GetTypeHash(const FInventoryItemHandle& Key);

/**
 * UObject representing a concrete inventory entry bound to a `UPA_ShopItem`.
 * Manages stack count, slot assignment, and all GAS-related behavior for the
 * item, including applying/removing equipped effects, granting an ability,
 * activating it, and exposing cooldown/mana cost queries for UI.
 */
UCLASS()
class UInventoryItem : public UObject
{
    GENERATED_BODY()
public:
    FOnAbilityCanCastUpdatedDelegate OnAbilityCanCastUpdated;
    // return true is was able to add
    bool AddStackCount();

    // returns true if the stack is not empty after reducing
    bool ReduceStackCount();

	// retruns true if was able to set
	bool SetStackCount(int NewStackCount);

	bool IsStackFull() const;

	bool IsForItem(const UPA_ShopItem* Item) const;
	bool IsGrantintAbility(TSubclassOf<class UGameplayAbility> AbilityClass) const;
	bool IsGrantingAnyAbility() const;

	UInventoryItem();
	bool IsValid() const;
	void InitItem(const FInventoryItemHandle& NewHandle, const UPA_ShopItem* NewShopItem, UAbilitySystemComponent* AbilitySystemComponent);
	const UPA_ShopItem* GetShopItem() const { return ShopItem; }
	FInventoryItemHandle GetHandle() const { return Handle; }

	bool TryActivateGrantedAbility();
	void ApplyConsumeEffect();
	void RemoveGASModifications();
	FORCEINLINE int GetStackCount() const { return StackCount; }
	void SetSlot(int NewSlot);
	int GetItemSlot() const { return Slot; }

	float GetAbilityCooldownTimeRemaining() const;
	float GetAbilityCooldownDuration() const;
	float GetAbilityManaCost() const;
	bool CanCastAbility() const;
	FGameplayAbilitySpecHandle GetGrantedAbilitySpecHandle() const { return GrantedAbiltiySpecHandle; }
	void SetGrantedAbilitySpecHandle(FGameplayAbilitySpecHandle SpecHandle) { GrantedAbiltiySpecHandle = SpecHandle; }
private:
	void ApplyGASModifications();
	UAbilitySystemComponent* OwnerAbilitySystemComponent;
	void ManaUpdated(const FOnAttributeChangeData& ChangeData);
	UPROPERTY()
	const UPA_ShopItem* ShopItem;
	FInventoryItemHandle Handle;
	int StackCount;
	int Slot;

	FActiveGameplayEffectHandle AppliedEquipedEffectHandle;
	FGameplayAbilitySpecHandle GrantedAbiltiySpecHandle;
};
