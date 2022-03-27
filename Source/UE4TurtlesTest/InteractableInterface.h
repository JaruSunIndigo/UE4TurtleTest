// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable, Category = "Turtles|Interfaces")
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class UE4TURTLESTEST_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Turtles|Interacting")
	void PerformInteract();

	virtual void PerformInteract_Implementation();
};
