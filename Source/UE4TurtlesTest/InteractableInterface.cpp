// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableInterface.h"

// Add default functionality here for any IInteractableInterface functions that are not pure virtual.

void IInteractableInterface::PerformInteract_Implementation() {
	UE_LOG(LogTemp, Warning, TEXT("%s [%i] : %s unimplemented"), __FILE__, __LINE__, __FUNCTION__);
}