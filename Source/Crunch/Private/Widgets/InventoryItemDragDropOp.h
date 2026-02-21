// Fill out your copyright notice in the Description page of Project Settings.

// ----------------------------------------------------------------------------
// File: InventoryItemDragDropOp.h
// Purpose: Custom UDragDropOperation that carries a reference to the source
//          InventoryItemWidget and provides a visual during drag.
// ----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryItemDragDropOp.generated.h"

class UInventoryItemWidget;
class UItemWidget;
/**
 * Drag-drop operation for inventory items. The operation stores which widget
 * initiated the drag so drop targets can query source slot/item. A lightweight
 * drag visual (derived from UItemWidget) can be instantiated to follow cursor.
 */
UCLASS()
class UInventoryItemDragDropOp : public UDragDropOperation
{
    GENERATED_BODY()
public:
    /** Sets the widget that started the drag; also configures drag visual */
    void SetDraggedItem(UInventoryItemWidget* DraggedItem);

private:
    /** Optional widget class used as drag visual thumbnail */
    UPROPERTY(EditDefaultsOnly, Category = "Visual")
    TSubclassOf<UItemWidget> DragVisualClass;
};
