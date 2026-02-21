// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayerController.generated.h"

/**
 * In-game PlayerController that owns gameplay UI and inputs, replicates a
 * FGenericTeamId, and coordinates with its possessed `ACPlayerCharacter`.
 */
UCLASS()
class ACPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
    GENERATED_BODY()
public:
    // only called on the server
    /**
     * Called when the PlayerController possesses a new Pawn.
     * @param NewPawn The Pawn being possessed.
     */
    void OnPossess(APawn* NewPawn) override;

    // only called on the client, also on the linstening server.
    /**
     * Called when the client acknowledges possession of a new Pawn.
     * @param NewPawn The Pawn being possessed.
     */
    void AcknowledgePossession(APawn* NewPawn) override;
    /**
     * Assigns the Team Agent to the given TeamID.
     * @param NewTeamID The new TeamID to assign.
     */
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
    
    /**
     * Retrieves the team identifier in the form of FGenericTeamId.
     * @return The current TeamID.
     */
    virtual FGenericTeamId GetGenericTeamId() const override;
    /**
     * Gets the lifetime replicated properties for this class.
     * @param OutLifetimeProps The array to fill with lifetime properties.
     */
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
    /**
     * Sets up the input component for this PlayerController.
     */
    virtual void SetupInputComponent() override;
    /**
     * Called when the match finishes.
     * @param ViewTarget The target actor to view.
     * @param WiningTeam The winning team ID.
     */
    void MatchFinished(AActor* ViewTarget, int WiningTeam);


private:
    /**
     * Client-side function to handle match finish.
     * @param ViewTarget The target actor to view.
     * @param WiningTeam The winning team ID.
     */
    UFUNCTION(Client, Reliable)
    void Client_MatchFinished(AActor* ViewTarget, int WiningTeam);

    /**
     * Spawns the gameplay widget.
     */
    void SpawnGameplayWidget();

    /**
     * The duration of the match finish view blend time.
     */
    UPROPERTY(EditDefaultsOnly, Category="View")
    float MatchFinishViewBlendTimeDuration = 2.f;

    /**
     * The possessed ACPlayerCharacter.
     */
    UPROPERTY()
    class ACPlayerCharacter* CPlayerCharacter;

    /**
     * The class of the gameplay widget to spawn.
     */
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UGameplayWidget> GameplayWidgetClass;

    /**
     * The spawned gameplay widget.
     */
    UPROPERTY()
    class UGameplayWidget* GameplayWidget;

    /**
     * The replicated TeamID.
     */
    UPROPERTY(Replicated)
    FGenericTeamId TeamID;

    /**
     * The input mapping context for UI inputs.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputMappingContext* UIInputMapping;

    /**
     * The input action for toggling the shop.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* ShopToggleInputAction;

    /**
     * The input action for toggling the gameplay menu.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* ToggleGameplayMenuAction;

    /**
     * Toggles the shop.
     */
    UFUNCTION()
    void ToggleShop();
    
    /**
     * Toggles the gameplay menu.
     */
    UFUNCTION()
    void ToggleGameplayMenu();
    /**
     * Shows the win/lose state.
     */
    void ShowWinLoseState();
};
