// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: MenuPlayerController.h
// Purpose: Base PlayerController used in menu contexts (main menu, lobby).
//          Spawns/owns a UUserWidget and updates it on join/rep events.
// ----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * Minimal PlayerController that bootstraps a menu widget on BeginPlay and
 * after PlayerState replication, providing a shared base for main menu and
 * lobby controllers.
 */
UCLASS()
class AMenuPlayerController : public APlayerController
{
    GENERATED_BODY()
public: 
    /**
     * Called when the game starts or when spawned.
     */
    virtual void BeginPlay() override;

    /**
     * Called when PlayerState is replicated.
     */
    virtual void OnRep_PlayerState() override;

private:
    /**
     * The class of the menu widget to spawn.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Menu")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    /**
     * The spawned menu widget.
     */
    UPROPERTY()
    UUserWidget* MenuWidget;

    /**
     * Spawns the menu widget.
     */
    void SpawnWidget();
};
