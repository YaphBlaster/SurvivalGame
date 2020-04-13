// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

USInteractionComponent::USInteractionComponent()
{
	//Save CPU by turning off tick
	SetComponentTickEnabled(false);

	InteractionTime = 0.f;
	InteractionDistance = 200.f;
	InteractableNameText = FText::FromString("Interactable Object");
	InteractableActionText = FText::FromString("Interact");
	bAllowMultipleInteractors = true;

	//UWidgetComponent properties
	Space = EWidgetSpace::Screen;

	//How big the UI is going to be
	DrawSize = FIntPoint(400, 100);

	bDrawAtDesiredSize = true;

	SetActive(true);

	//Set to hidden because we want to choose when we see the component
	SetHiddenInGame(true);
}
