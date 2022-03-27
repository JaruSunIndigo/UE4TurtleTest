// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "InteractableInterface.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AUE4TurtlesTestCharacter

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Set default interation range
	InteractionRayLength = 500.f;
}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind interact event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::OnInteract);

	// Bind interact event
	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &APlayerCharacter::OnExit);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}

//Custom interact action
void APlayerCharacter::OnInteract() {

	FHitResult RayHitResult;

	//Cast ray from Camera center
	FVector RayStartPoint = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForewardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector RayEndPoint = (ForewardVector * InteractionRayLength) + RayStartPoint;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this->GetOwner());

	bool bInteractSuccess = false;

	UWorld* World = GetWorld();

	if (World != nullptr) {

		if (World->LineTraceSingleByChannel(RayHitResult, RayStartPoint, RayEndPoint, ECC_Visibility, CollisionQueryParams)) {

			//DrawDebugLine(GetWorld(), RayStartPoint, RayEndPoint, FColor::Blue, true);

			TWeakObjectPtr<AActor> Actor = RayHitResult.Actor;

			if (Actor.IsValid()) {

				//Find actor implements interface UInteractableInterface
				if (Actor->Implements<UInteractableInterface>()) {

					bInteractSuccess = true;

					AActor* ActorRef = Actor.Get();

					if (ActorRef != nullptr) {
						IInteractableInterface::Execute_PerformInteract(ActorRef);
					}
				}

			}

		}
		else
		{
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("To far from target to interact"));
			}
		}



		//Play interaction sounds
		if (bInteractSuccess)
		{
			if (IsValid(AllowInteractionSound)) {
				UGameplayStatics::PlaySound2D(GetWorld(), AllowInteractionSound, 1.0f, 1.0f);
			}
		}
		else
		{
			if (IsValid(DenyInteractionSound)) {
				UGameplayStatics::PlaySound2D(GetWorld(), DenyInteractionSound, 1.0f, 1.0f);
			}
		}

	}
}

void APlayerCharacter::OnExit() {
	
	UWorld* World = GetWorld();

	if (World != nullptr) {

		if (World->IsGameWorld()) {

			FGenericPlatformMisc::RequestExit(false);

		}

	}
}

void APlayerCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (!TouchItem.bIsPressed)
	{
		TouchItem.bIsPressed = true;
		TouchItem.FingerIndex = FingerIndex;
		TouchItem.Location = Location;
		TouchItem.bMoved = false;
	}
}

void APlayerCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed)
	{
		TouchItem.bIsPressed = false;
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool APlayerCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	bool result = false;

	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APlayerCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &APlayerCharacter::EndTouch);

		result = true;
	}

	return result;
}
