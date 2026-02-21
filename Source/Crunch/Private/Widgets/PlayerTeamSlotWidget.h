// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// ----------------------------------------------------------------------------
// File: PlayerTeamSlotWidget.h
// Purpose: Displays a single player's slot (name and character icon) in the
//          team layout. Handles hover feedback and slot updates.
// ----------------------------------------------------------------------------
#include "PlayerTeamSlotWidget.generated.h"

class UPA_CharacterDefination;
/**
 * A lobby/team UI element showing the player's nickname and selected character.
 * Plays a hover animation and supports updating the slot content on demand.
 */
UCLASS()
class UPlayerTeamSlotWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    /** Update both player name and character icon for this slot */
    void UpdateSlot(const FString& PlayerName, const UPA_CharacterDefination* CharacterDefination);

    virtual void NativeOnMouseEnter( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
    virtual void NativeOnMouseLeave( const FPointerEvent& InMouseEvent ) override;

private: 
    /** Hover animation played when mouse enters/leaves */
    UPROPERTY(Transient, meta=(BindWidgetAnim))
    class UWidgetAnimation* HoverAnim;

    /** Character icon image; driven by a material parameter */
    UPROPERTY(meta=(BindWidget))
    class UImage* PlayerCharacterIcon;

    /** Player nickname text */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* NameText;

    /** Material parameter names for icon texture and empty state */
    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    FName CharacterIconMatParamName = "Icon";

    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    FName CharacterEmptyMatParamName = "Empty";

    FString CachedPlayerNameStr;
    FString CachedCharacterNameStr;

    /** Refresh NameText using cached strings */
    void UpdateNameText();
};
