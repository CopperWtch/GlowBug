// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RandomWalker.h"
#include "BlockGrid.generated.h"

/**
 * Class to spawn the blocks
 */
UCLASS(minimalapi)
//GLOWBUG_API
class ABlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;
private:
	//number of Blocks in the level
	int32 countBlocks;
	//every block in the grid is spawned
	bool bIsCompleted;
	//generates  locations for the blocks
	RandomWalker* walker;
	//keeps track of the locations
	TArray<FVector> blockLocations;
	
public:
	ABlockGrid(const FObjectInitializer& ObjectInitializer);

	/** Number of blocks along each side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int32 Size;
	
	/** Spacing of blocks */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		float BlockSpacing;
	
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

	//returns number of blocks in the grid without the exit block
	int32 GetCountBlocks()
	{
		return countBlocks;
	}
	//set number of blocks in the grid without the exit block
	void SetCountBlocks(int32 count)
	{
		countBlocks = count;
	}

	//get/Set bIsCompleted
	bool IsCompleted(){ return bIsCompleted; }
	void SetIsCompleted(bool state){ bIsCompleted = state; }
};
