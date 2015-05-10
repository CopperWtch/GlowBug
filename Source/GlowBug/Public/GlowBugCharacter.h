// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "DefaultBlock.h"
#include <vector>
#include "GlowBugCharacter.generated.h"


//enum to store the current movement
enum class ECharacterMovement : short
{
	EForward,
	EBack,
	ERight,
	ELeft,
	ENone
};

UCLASS(config=Game)
class AGlowBugCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGlowBugCharacter(const FObjectInitializer& ObjectInitializer);



protected:

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

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void OnStepNorth();
	void OnStepSouth();
	void OnStepEast();
	void OnStepWest();
	void OnMovementReleased();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:

	
	//Array of Actors the Character is currently colliding with
	TArray<AActor*> CollectedActors;

	virtual void Tick(float DeltaSeconds) OVERRIDE;
	void StepOff();
	

};

