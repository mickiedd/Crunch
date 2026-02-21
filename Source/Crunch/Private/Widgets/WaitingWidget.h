// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
// ----------------------------------------------------------------------------
// File: WaitingWidget.h
// Purpose: Generic modal waiting/notification widget with optional cancel.
//          Exposes a button clicked event for external binding.
// ----------------------------------------------------------------------------
#include "WaitingWidget.generated.h"

/**
 * Displays a text message while waiting for an async operation (e.g., session
 * search/join). Optionally shows a cancel button and provides an accessor to
 * bind to its click event.
 */
UCLASS()
class UWaitingWidget : public UUserWidget
{
    GENERATED_BODY()
    
public: 
    virtual void NativeConstruct() override;

    /** Clears prior bindings and returns the cancel button's click event */
    FOnButtonClickedEvent& ClearAndGetButtonClickedEvent();
    /** Update displayed message and whether cancel is enabled/visible */
    void SetWaitInfo(const FText& WaitInfo, bool bAllowCancel = false);

private:
    /** Message text shown to the user */
    UPROPERTY(meta=(BindWidget))
    class UTextBlock* WaitInfoText;

    /** Optional cancel button */
    UPROPERTY(meta=(BindWidget))
    UButton* CancelBtn;
};
