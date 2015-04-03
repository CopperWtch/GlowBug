// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "GlowBug.h"
#include "Engine.h"
#include "GlowBugCharacter.h"


//////////////////////////////////////////////////////////////////////////
// AGlowBugCharacter

AGlowBugCharacter::AGlowBugCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	currentMovement = ECharacterMovement::ENone;
	bCanMoveForward = true;
	bCanMoveBack = true;
	bCanMoveRight = true;
	bCanMoveLeft = true;

}

//////////////////////////////////////////////////////////////////////////
// Input

void AGlowBugCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("StepNorth", IE_Released, this, &AGlowBugCharacter::OnStepNorth);
	InputComponent->BindAction("StepSouth", IE_Released, this, &AGlowBugCharacter::OnStepSouth);
	InputComponent->BindAction("StepEast", IE_Released, this, &AGlowBugCharacter::OnStepEast);
	InputComponent->BindAction("StepWest", IE_Released, this, &AGlowBugCharacter::OnStepWest);
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

void AGlowBugCharacter::OnStepNorth()
{

	//this->SetActorLocation(this->GetActorLocation() + FVector(120.0f,0,0));

	if (currentMovement == ECharacterMovement::ENone)
	{	
		newLocation = this->GetActorLocation() + FVector(120.0f, 0,0);
		currentMovement = ECharacterMovement::EForward;
	}

}
void AGlowBugCharacter::OnStepSouth()
{
	//this->SetActorLocation(this->GetActorLocation() + FVector(-120.f, 0, 0));
	if (currentMovement == ECharacterMovement::ENone)
	{
		newLocation = this->GetActorLocation() + FVector(-120.0f, 0, 0);
		currentMovement = ECharacterMovement::EBack;
	}
}
void AGlowBugCharacter::OnStepEast()
{

	//this->SetActorLocation(this->GetActorLocation() + FVector(120.0f,0,0));
	if (currentMovement == ECharacterMovement::ENone)
	{
		newLocation = this->GetActorLocation() + FVector(0, 120.f, 0);
		currentMovement = ECharacterMovement::ERight;
	}
}
void AGlowBugCharacter::OnStepWest()
{
	//this->SetActorLocation(this->GetActorLocation() + FVector(-120.f, 0, 0));
	if (currentMovement == ECharacterMovement::ENone)
	{
		newLocation = this->GetActorLocation() + FVector(0, -120.f, 0);
		currentMovement = ECharacterMovement::ELeft;
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

			block->bIsColliding=true;

			if (block->GetBlockNorth())
				bCanMoveForward = true;
			else
				bCanMoveForward = false;

			if (block->GetBlockSouth())
				bCanMoveBack = true;
			else
				bCanMoveBack = false;

			if (block->GetBlockEast())
				bCanMoveRight = true;
			else
				bCanMoveRight = false;

			if (block->GetBlockWest())
				bCanMoveLeft = true;
			else
				bCanMoveLeft = false;

		}
	}

}

void AGlowBugCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//call in every update to check for collision


	switch (currentMovement)
	{

	case ECharacterMovement::EForward:

		if (bCanMoveForward)
		{		
		if (GetActorLocation().X <= newLocation.X + 3 &&
			GetActorLocation().X >= newLocation.X - 3)
			currentMovement = ECharacterMovement::ENone;
		else
			MoveForward(1.f);
		}
		else
			currentMovement = ECharacterMovement::ENone;


		break;
	case ECharacterMovement::EBack:

		if (bCanMoveBack)
		{
		if (GetActorLocation().X <= newLocation.X + 3 &&
			GetActorLocation().X >= newLocation.X - 3)
			currentMovement = ECharacterMovement::ENone;
		else
			MoveForward(-1.f);
		}
		else
			currentMovement = ECharacterMovement::ENone;
		
		break;
	case ECharacterMovement::ERight:

		if (bCanMoveRight)
		{
			if (GetActorLocation().Y<= newLocation.Y +3 &&
				GetActorLocation().Y>= newLocation.Y- 3)
				currentMovement = ECharacterMovement::ENone;
			else
				MoveRight(1.f);
		}	
		else
			currentMovement = ECharacterMovement::ENone;

		break;
	case ECharacterMovement::ELeft:

		if (bCanMoveLeft)
		{
			if (GetActorLocation().Y <= newLocation.Y + 3 &&
				GetActorLocation().Y >= newLocation.Y - 3)
				currentMovement = ECharacterMovement::ENone;
			else
				MoveRight(-1.f);
		}
		else
			currentMovement = ECharacterMovement::ENone;

		break;
	case ECharacterMovement::ENone:
		StepOff();

		break;

	}

}