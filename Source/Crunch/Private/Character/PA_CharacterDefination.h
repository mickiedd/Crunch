// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GAS/CGameplayAbilityTypes.h"
// ----------------------------------------------------------------------------
// File: PA_CharacterDefination.h
// Purpose: Defines a Primary Data Asset that describes a character selection
//          entry: display name/icon, character class to spawn, display anim BP
//          and ability loadout bindings.
// Key API:
//  - Primary Asset: GetPrimaryAssetId and static GetCharacterDefinationAssetType
//    provide asset registry identification.
//  - Loading helpers: LoadIcon, LoadCharacterClass, LoadDisplayAnimationBP,
//    LoadDisplayMesh for asset resolution at runtime.
//  - Abilities: GetAbilities returns the mapping from ECAbilityInputID to
//    UGameplayAbility classes used by the character.
// ----------------------------------------------------------------------------
#include "PA_CharacterDefination.generated.h"

class ACCharacter;
class UGameplayAbility;
/**
 * Primary data asset bundling character definition for selection/spawn menus.
 * Stores soft references to content to keep cook sizes low and enable async
 * loading. Exposes convenience loaders for UI and spawning code paths.
 */
UCLASS()
class UPA_CharacterDefination : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	static FPrimaryAssetType GetCharacterDefinationAssetType();

	FString GEtCharacterDisplayName() const { return CharacterName; }
	UTexture2D* LoadIcon() const;
	TSubclassOf<ACCharacter> LoadCharacterClass() const;
	TSubclassOf<UAnimInstance> LoadDisplayAnimationBP() const;
	class USkeletalMesh* LoadDisplayMesh() const;
	const TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>>* GetAbilities() const;

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Character")
	FString CharacterName;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftObjectPtr<UTexture2D> CharacterIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftClassPtr<ACCharacter> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSoftClassPtr<UAnimInstance> DisplayAnimBP;
};
