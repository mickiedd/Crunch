// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
// ----------------------------------------------------------------------------
// File: GameplayMenu.h
// Purpose: In-game pause/options menu with Resume, Back-to-MainMenu, and Quit.
//          Exposes button events and allows setting a dynamic title.
// ----------------------------------------------------------------------------
#include "GameplayMenu.generated.h"

/**
 * Pause menu displayed over gameplay. `UGameplayWidget` owns and toggles it.
 * Blueprint implements the navigation/visibility, while this class wires up
 * button callbacks and exposes a delegate for Resume.
 */
UCLASS()
class UGameplayMenu : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    /** Returns a delegate reference to bind Resume/close behavior */
    FOnButtonClickedEvent& GetResumeButtonClickedEventDelegate();
    /** Sets the text title shown at the top of the menu */
    void SetTitleText(const FString& NewTitle);
private:
    /** Title text block at the top of the menu */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* MenuTitle;

    /** Resume gameplay */
    UPROPERTY(meta=(BindWidget))
    class UButton* ResumeBtn;

    /** Return to the main menu map/flow */
    UPROPERTY(meta=(BindWidget))
    class UButton* MainMenuBtn;

    /** Quit the game application */
    UPROPERTY(meta=(BindWidget))
    class UButton* QuitGameBtn;

    UFUNCTION()
    void BackToMainMenu();

    UFUNCTION()
    void QuitGame();
};
