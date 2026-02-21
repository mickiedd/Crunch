// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
// ----------------------------------------------------------------------------
// File: SessionEntryWidget.h
// Purpose: Represents a single online session entry with name and a button to
//          select/join. Emits a delegate with the selected session ID string.
// ----------------------------------------------------------------------------
#include "SessionEntryWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSessionEntrySelected, const FString& /*SelectedSessionIdStr*/)
/**
 * UI entry for discoverable sessions. Initialize with name and session ID; on
 * click it notifies listeners through OnSessionEntrySelected.
 */
UCLASS()
class USessionEntryWidget : public UUserWidget
{
    GENERATED_BODY()
public: 
    virtual void NativeConstruct() override;
    /** Fired when this entry is clicked by the user */
    FOnSessionEntrySelected OnSessionEntrySelected;
    /** Configure this entry with the session display name and ID */
    void InitializeEntry(const FString& Name, const FString& SessionIdStr);
    FORCEINLINE FString GetCachedSessionIdStr() const { return CachedSessionIdStr; }
private:
    /** Session name text label */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* SessionNameText;

    /** Button to select/join this session */
    UPROPERTY(meta=(BindWidget))
    class UButton* SessionBtn;

    /** Cached session ID string associated with this entry */
    FString CachedSessionIdStr;

    UFUNCTION()
    void SessionEntrySelected();
};
