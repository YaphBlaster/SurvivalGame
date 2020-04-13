// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SInteractionComponent.generated.h"



/**
 * In order to be used in blueprints you must add the parameters below
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SURVIVALGAME_API USInteractionComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	// Constructor
	USInteractionComponent();

public:
	//The Time the player must hold the interact key to interact with this object
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionTime;

	//The max distance the player can be away from this actor before you can interact
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionDistance;

	//The name that will come up when the player looks at the interactable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText InteractableNameText;

	//The verb that describes how the interaction works, ie "Sit" for a chair, "Eat" for food, "Light" for a fireplace
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText InteractableActionText;

	//Whether we allow multiple players to interact with the item, or just one at any given time.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bAllowMultipleInteractors;;

	
};
