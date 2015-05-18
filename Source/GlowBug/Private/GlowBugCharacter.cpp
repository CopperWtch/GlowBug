/**
Game Development Project
GlowBugCharacter.cpp
Purpose: Responsible for character movement and collisions with the character

@author Sarah Bulk
*/

#include "GlowBug.h"
#include "Engine.h"
#include "GlowBugCharacter.h"


//////////////////////////////////////////////////////////////////////////////////////
//Constructor
//////////////////////////////////////////////////////////////////////////////////////
AGlowBugCharacter::AGlowBugCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);



	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; //movement direction	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // rotation rate
	
	//Set default values
	currentMovement = ECharacterMovement::ENone; //Character stands still in the beginning
	bCanMoveForward = true;
	bCanMoveBack = true;
	bCanMoveRight = true;
	bCanMoveLeft = true;

}

//////////////////////////////////////////////////////////////////////////////////////
//Configure Input
//////////////////////////////////////////////////////////////////////////////////////
void AGlowBugCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	//Input Bindings for the beginning of the character movement
	InputComponent->BindAction("StepNorth", IE_Pressed, this, &AGlowBugCharacter::OnStepNorth);
	InputComponent->BindAction("StepSouth", IE_Pressed, this, &AGlowBugCharacter::OnStepSouth);
	InputComponent->BindAction("StepEast", IE_Pressed, this, &AGlowBugCharacter::OnStepEast);
	InputComponent->BindAction("StepWest", IE_Pressed, this, &AGlowBugCharacter::OnStepWest);

	//Input Bindings for the end of the character movement
	InputComponent->BindAction("StepNorth", IE_Released, this, &AGlowBugCharacter::OnMovementReleased);
	InputComponent->BindAction("StepSouth", IE_Released, this, &AGlowBugCharacter::OnMovementReleased);
	InputComponent->BindAction("StepEast", IE_Released, this, &AGlowBugCharacter::OnMovementReleased);
	InputComponent->BindAction("StepWest", IE_Released, this, &AGlowBugCharacter::OnMovementReleased);
}


//////////////////////////////////////////////////////////////////////////////////////
//Movement in X or Y direction
//////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////
//Methods to move the character in different directions
//////////////////////////////////////////////////////////////////////////////////////
void AGlowBugCharacter::OnStepNorth()
{
	//If the character isnt currently moving, allow for movement north
	if (currentMovement == ECharacterMovement::ENone)
	{	
		currentMovement = ECharacterMovement::EForward;
	}

}
void AGlowBugCharacter::OnStepSouth()
{
	//If the character isnt currently moving, allow for movement south
	if (currentMovement == ECharacterMovement::ENone)
	{
		currentMovement = ECharacterMovement::EBack;
	}
}
void AGlowBugCharacter::OnStepEast()
{
	//If the character isnt currently moving, allow for movement east
	if (currentMovement == ECharacterMovement::ENone)
	{
		currentMovement = ECharacterMovement::ERight;
	}
}
void AGlowBugCharacter::OnStepWest()
{
	//If the character isnt currently moving, allow for movement west
	if (currentMovement == ECharacterMovement::ENone)
	{
		currentMovement = ECharacterMovement::ELeft;
	}
}

void AGlowBugCharacter::OnMovementReleased()
{
	//If the character is currently moving, stop the movement
	if (currentMovement != ECharacterMovement::ENone)
	{
		currentMovement = ECharacterMovement::ENone;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//Check for collision
//////////////////////////////////////////////////////////////////////////////////////
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

			//Set in which directions the character can move at this point
			if (!block->GetBlockNorth() &&
				this->GetActorLocation().X>block->GetActorLocation().X)
				bCanMoveForward = false;
			else
				bCanMoveForward = true;

			if (!block->GetBlockSouth() &&
				this->GetActorLocation().X<block->GetActorLocation().X)
				bCanMoveBack = false;
			else
				bCanMoveBack = true;

			if (!block->GetBlockEast() &&
				this->GetActorLocation().Y>block->GetActorLocation().Y)
				bCanMoveRight = false;
			else
				bCanMoveRight = true;

			if (!block->GetBlockWest() &&
				this->GetActorLocation().Y<block->GetActorLocation().Y)
				bCanMoveLeft = false;
			else
				bCanMoveLeft = true;

		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//Called on every update
//////////////////////////////////////////////////////////////////////////////////////
void AGlowBugCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//call in every update to check for collision
	StepOff();

	//Check if the player CAN move or if they are close to an edge
	switch (currentMovement)
	{
	
	//check for movement to the north
	case ECharacterMovement::EForward:

		if (bCanMoveForward)
		{		
			MoveForward(1.f);
		}
		else
		{			
			currentMovement = ECharacterMovement::ENone;
		}



		break;
	
	//check for movement to the south
	case ECharacterMovement::EBack:

		if (bCanMoveBack)
		{
			MoveForward(-1.f);
		}
		else
		{	
			currentMovement = ECharacterMovement::ENone;
		}

		
		break;

	//check for movement to the east
	case ECharacterMovement::ERight:

		if (bCanMoveRight)
		{
				MoveRight(1.f);
		}	
		else
		{			
			currentMovement = ECharacterMovement::ENone;
		}


		break;

	//check for movement to the west
	case ECharacterMovement::ELeft:

		if (bCanMoveLeft)
		{
			MoveRight(-1.f);
		}
		else
		{
			currentMovement = ECharacterMovement::ENone;
		}
			

		break;
	case ECharacterMovement::ENone:

		break;

	}

}