/**
Game Development Project
GlowBugCharacter.h
Purpose: Header file for GlowBugCharacter.cpp

@author Sarah Bulk
*/

#pragma once
#include "GameFramework/Character.h"
#include "DefaultBlock.h"
#include <vector>
#include "GlowBugCharacter.generated.h"


//////////////////////////////////////////////////////////////////////////////////////
//enum to store the current direction of the character movement
//////////////////////////////////////////////////////////////////////////////////////
enum class ECharacterMovement : short
{
	EForward,
	EBack,
	ERight,
	ELeft,
	ENone
};


//////////////////////////////////////////////////////////////////////////////////////
//Default Character class declarations
//////////////////////////////////////////////////////////////////////////////////////
UCLASS(config = Game)
class AGlowBugCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//constructor
	AGlowBugCharacter(const FObjectInitializer& ObjectInitializer);



protected:

	//current movement direction
	ECharacterMovement currentMovement;

	//whether or not the character can move in different directions at this point in time
	bool bCanMoveForward;
	bool bCanMoveBack;
	bool bCanMoveRight;
	bool bCanMoveLeft;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	//methods handling the movement in different directions
	void OnStepNorth();
	void OnStepSouth();
	void OnStepEast();
	void OnStepWest();
	void OnMovementReleased();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:


	//Array of Actors the Character is currently colliding with
	TArray<AActor*> CollectedActors;

	//Tick is called on every update
	virtual void Tick(float DeltaSeconds) OVERRIDE;

	//Checks for collision
	void StepOff();


};

