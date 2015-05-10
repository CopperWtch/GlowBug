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

	InputComponent->BindAction("StepNorth", IE_Pressed, this, &AGlowBugCharacter::OnStepNorth);
	InputComponent->BindAction("StepSouth", IE_Pressed, this, &AGlowBugCharacter::OnStepSouth);
	InputComponent->BindAction("StepEast", IE_Pressed, this, &AGlowBugCharacter::OnStepEast);
	InputComponent->BindAction("StepWest", IE_Pressed, this, &AGlowBugCharacter::OnStepWest);

	InputComponent->BindAction("StepNorth", IE_Released, this, &AGlowBugCharacter::OnMovementReleased);
	InputComponent->BindAction("StepSouth", IE_Released, this, &AGlowBugCharacter::OnMovementReleased);
	InputComponent->BindAction("StepEast", IE_Released, this, &AGlowBugCharacter::OnMovementReleased);
	InputComponent->BindAction("StepWest", IE_Released, this, &AGlowBugCharacter::OnMovementReleased);
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
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("PRESSED"));
	//this->SetActorLocation(this->GetActorLocation() + FVector(120.0f,0,0));

	if (currentMovement == ECharacterMovement::ENone)
	{	
		currentMovement = ECharacterMovement::EForward;
	}

}
void AGlowBugCharacter::OnStepSouth()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("PRESSED"));
	//this->SetActorLocation(this->GetActorLocation() + FVector(-120.f, 0, 0));
	if (currentMovement == ECharacterMovement::ENone)
	{
		currentMovement = ECharacterMovement::EBack;
	}
}
void AGlowBugCharacter::OnStepEast()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("PRESSED"));
	//this->SetActorLocation(this->GetActorLocation() + FVector(120.0f,0,0));
	if (currentMovement == ECharacterMovement::ENone)
	{
		currentMovement = ECharacterMovement::ERight;
	}
}
void AGlowBugCharacter::OnStepWest()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("PRESSED"));
	//this->SetActorLocation(this->GetActorLocation() + FVector(-120.f, 0, 0));
	if (currentMovement == ECharacterMovement::ENone)
	{
		currentMovement = ECharacterMovement::ELeft;
	}
}

void AGlowBugCharacter::OnMovementReleased()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("RELEASED"));
	//this->SetActorLocation(this->GetActorLocation() + FVector(-120.f, 0, 0));
	if (currentMovement != ECharacterMovement::ENone)
	{
		currentMovement = ECharacterMovement::ENone;
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

void AGlowBugCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//call in every update to check for collision
	StepOff();

	switch (currentMovement)
	{

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