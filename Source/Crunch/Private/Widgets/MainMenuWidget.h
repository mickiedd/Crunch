// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "OnlineSessionSettings.h"
// ----------------------------------------------------------------------------
// File: MainMenuWidget.h
// Purpose: Main menu UI that handles login, session creation/joining, and
//          switching between main, session, and waiting views.
// ----------------------------------------------------------------------------
#include "MainMenuWidget.generated.h"

/**
 * Landing screen that integrates with OnlineSubsystem via `UCGameInstance` to
 * log in, browse sessions, create or join a session, and show waiting state.
 */
UCLASS()
class UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()
    
public: 
    /**
     * NativeConstruct is called when the widget is initialized.
     */
    virtual void NativeConstruct() override;
    
    /******************************/ 
    /*           Main             */
    /******************************/ 
private:
    /**
     * Switcher for main menu widgets.
     */
    UPROPERTY(meta=(BindWidget))
    class UWidgetSwitcher* MainSwitcher;

    /**
     * Reference to the game instance.
     */
    UPROPERTY()
    class UCGameInstance* CGameInstance;

    /**
     * Switches to the main widget.
     */
    void SwitchToMainWidget();

    /**
     * Root widget for the main menu.
     */
    UPROPERTY(meta=(BindWidget))
    class UWidget* MainWidgetRoot;

    /******************************/ 
    /*           Session          */
    /******************************/ 
    /**
     * Button to create a new session.
     */
    UPROPERTY(meta=(BindWidget))
    class UButton* CreateSessionBtn;

    /**
     * Text field for the new session name.
     */
    UPROPERTY(meta=(BindWidget))
    class UEditableText* NewSessionNameText;

    /**
     * Called when the create session button is clicked.
     */
    UFUNCTION()
    void CreateSesisonBtnClicked();

    /**
     * Cancels the session creation process.
     */
    UFUNCTION()
    void CancelSessionCreation();

    /**
     * Called when the new session name text changes.
     */
    UFUNCTION()
    void NewSessionNameTextChanged(const FText& NewText);

    /**
     * Called when joining a session fails.
     */
    void JoinSessionFailed();

    /**
     * Updates the lobby list with the given search results.
     */
    void UpdateLobbyList(const TArray<FOnlineSessionSearchResult>& SearchResults);

    /**
     * Scroll box for the session list.
     */
    UPROPERTY(meta=(BindWidget))
    class UScrollBox* SessionScrollBox;

    /**
     * Button to join a session.
     */
    UPROPERTY(meta=(BindWidget))
    class UButton* JoinSessionBtn;

    /**
     * Class of the session entry widget.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Session")
    TSubclassOf<class USessionEntryWidget> SessionEntryWidgetClass;

    /**
     * ID of the currently selected session.
     */
    FString CurrentSelectedSessionId = "";

    /**
     * Called when the join session button is clicked.
     */
    UFUNCTION()
    void JoinSessionBtnClicked();

    /**
     * Called when a session entry is selected.
     */
    void SessionEntrySelected(const FString& SelectedEntryIdStr);

    /******************************/ 
    /*           Login             */
    /******************************/ 
private:
    /**
     * Root widget for the login menu.
     */
    UPROPERTY(meta=(BindWidget))
    class UWidget* LoginWidgetRoot;

    /**
     * Button to log in.
     */
    UPROPERTY(meta=(BindWidget))
    class UButton* LoginBtn;

    /**
     * Called when the login button is clicked.
     */
    UFUNCTION()
    void LoginBtnClicked();

    /**
     * Called when the login process is completed.
     */
    void LoginCompleted(bool bWasSuccessful, const FString& PlayerNickname, const FString& ErrorMsg);

    /******************************/ 
    /*           Waiting          */
    /******************************/ 
private:
    /**
     * Waiting widget.
     */
    UPROPERTY(meta=(BindWidget))
    class UWaitingWidget* WaitingWidget;

    /**
     * Switches to the waiting widget with the given wait info.
     */
    FOnButtonClickedEvent& SwitchToWaitingWidget(const FText& WaitInfo, bool bAllowCancel = false);
};
