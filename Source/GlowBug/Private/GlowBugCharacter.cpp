// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "GlowBug.h"
#include "Engine.h"
#include "GlowBugCharacter.h"
#include "DefaultBlock.h"

//////////////////////////////////////////////////////////////////////////
// AGlowBugCharacter

AGlowBugCharacter::AGlowBugCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	//CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	//CameraBoom->AttachTo(RootComponent);
	//CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	//FollowCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	//FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGlowBugCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AGlowBugCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGlowBugCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//InputComponent->BindAxis("TurnRate", this, &AGlowBugCharacter::TurnAtRate);
	//InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//InputComponent->BindAxis("LookUpRate", this, &AGlowBugCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AGlowBugCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AGlowBugCharacter::TouchStopped);
}


void AGlowBugCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AGlowBugCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AGlowBugCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGlowBugCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGlowBugCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGlowBugCharacter::MoveRight(float Value)
{

	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//Called constantly to check for collision
void AGlowBugCharacter::StepOff()
{
	//Returns all the actors colliding with the character
	CapsuleComponent->GetOverlappingActors(CollectedActors);

	//iterate through them 
	for (int32 i = 0; i < CollectedActors.Num(); i++)
	{
		//Cast the AActor Array to ADefaultBlock
		ADefaultBlock* block = Cast<ADefaultBlock>(CollectedActors[i]);

		//make sure the block is active & not destroyed already
		if (block != NULL && !block->IsPendingKill() && block->bIsActive)
		{
			//if (block->GetBlockEast())
			//{
			//	FString TheFloatStr = FString::SanitizeFloat(block->GetBlockEast()->GetActorLocation().X);
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *TheFloatStr);
			//}
			//if (block->GetBlockNorth())
			//{
			//	FString TheFloatStr = FString::SanitizeFloat(block->GetBlockNorth()->GetActorLocation().X);
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, *TheFloatStr);
			//}
			//if (block->GetBlockWest())
			//{
				//FString TheFloatStr = FString::SanitizeFloat(block->GetBlockWest()->GetActorLocation().X);
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, *TheFloatStr);
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("x: %f, y: %f"), block->GetBlockWest()->GetActorLocation().X, block->GetBlockWest()->GetActorLocation().Y));
			//}
			//if (block->GetBlockSouth())
			//{
			//	FString TheFloatStr = FString::SanitizeFloat(block->GetBlockSouth()->GetActorLocation().X);
			//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *TheFloatStr);
			//}

			block->bIsColliding=true;
		}
	}

}

void AGlowBugCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//call in every update to check for collision
	StepOff();
}