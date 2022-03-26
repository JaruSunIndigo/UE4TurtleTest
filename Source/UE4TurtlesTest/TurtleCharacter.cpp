// Fill out your copyright notice in the Description page of Project Settings.


#include "TurtleCharacter.h"

// Sets default values
ATurtleCharacter::ATurtleCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurtleCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurtleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATurtleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

