// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/TreeNodeInterface.h"
// ----------------------------------------------------------------------------
// File: ItemTreeWidget.h
// Purpose: Visualizes a directed item combination tree (e.g., shop recipes)
//          by laying out nodes on a canvas and drawing connections between
//          parent/child nodes.
// ----------------------------------------------------------------------------
#include "ItemTreeWidget.generated.h"

/**
 * Renders a craft/combination tree rooted at a center item. Given a node via
 * `ITreeNodeInterface`, it lays out upstream/downstream items and draws lines
 * to communicate combination relationships.
 */
UCLASS()
class UItemTreeWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    /** Draw a tree starting from the provided node interface as the center */
    void DrawFromNode(const ITreeNodeInterface* NodeInterface);
private:
    /** Lays out and draws one stream (upwards for components or downwards for outputs) */
    void DrawStream(
        bool bUpperStream, 
        const ITreeNodeInterface* StartingNodeInteface,
        UUserWidget* StartingNodeWidget, 
        class UCanvasPanelSlot* StartingNodeSlot,
        int StartingNodeDepth,
        float& NextLeafXPosition,
        TArray<UCanvasPanelSlot*>& OutStreamSlots
    );
    /** Clears previous nodes and connections */
    void ClearTree();
    /** Creates a widget for a given node and adds it to the canvas */
    UUserWidget* CreateWidgetForNode(const ITreeNodeInterface* Node, class UCanvasPanelSlot*& OutCanvasSlot);
    /** Draws a visual connection between two node widgets */
    void CreateConnection(const UUserWidget* From, UUserWidget* To);

    UPROPERTY(meta=(BindWidget))
    class UCanvasPanel* RootPanel;
    const UObject* CurrentCenterItem;

    UPROPERTY(EditDefaultsOnly, Category = "Tree")
    FVector2D NodeSize = FVector2D{ 60.f };

    UPROPERTY(EditDefaultsOnly, Category = "Tree")
    FVector2D NodeGap = FVector2D{ 16.f, 30.f};

    UPROPERTY(EditDefaultsOnly, Category = "Tree")
    FLinearColor ConnectionColor = FLinearColor{0.8f, 0.8f, 0.8f, 1.f};

    UPROPERTY(EditDefaultsOnly, Category = "Tree")
    float ConnectionThickness = 3.f;

    UPROPERTY(EditDefaultsOnly, Category = "Tree")
    FVector2D SourePortLocalPos = FVector2D{ 0.5f, 0.9f };

    UPROPERTY(EditDefaultsOnly, Category = "Tree")
    FVector2D DestinationPortLocalPos = FVector2D{ 0.5f, 0.1f };

    UPROPERTY(EditDefaultsOnly, Category = "Tree")
    FVector2D SourcePortDirection = FVector2D{ 0.f, 90.f };

    UPROPERTY(EditDefaultsOnly, Category = "Tree")
    FVector2D DestinationPortDirection = FVector2D{ 0.f, 90.f };
};
