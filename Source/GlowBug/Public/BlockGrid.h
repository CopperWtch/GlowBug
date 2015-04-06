// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <list>
#include <vector>
#include "DefaultBlock.h"
#include "ExitBlock.h"
#include "GlowBugGameMode.h"
#include "BlockGrid.generated.h"

using namespace std;

/**
 * Class to spawn the blocks
 */

struct Coordinate
{
	int x;
	int y;
};

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
	float maxY;
	float minY;
	float maxX;
	float minX;

	//every block in the grid is spawned
	bool bIsCompleted;

	ADefaultBlock* newBlock;
	ADefaultBlock* exitBlock;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BPClasses)
		UClass* newBlockBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BPClasses)
		UClass* exitBlockBP;

	ABlockGrid(const FObjectInitializer& ObjectInitializer);

	/** Number of blocks along each side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;
	
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

	//returns number of blocks in the grid without the exit block
	int32 GetCountBlocks()
	{
		return countBlocks;
	};
	//set number of blocks in the grid without the exit block
	void SetCountBlocks(int32 count)
	{
		countBlocks = count;
	};




	//get/Set bIsCompleted
	bool IsCompleted(){ return bIsCompleted; };
	void SetIsCompleted(bool state){ bIsCompleted = state; };


	//Generate the level

	bool grid[50][50];
	int steps[4];

	Coordinate GetNextPosition(vector<Coordinate> freeSpots, Coordinate currPos, int steps[4]);
	void GenerateLevel(int maxCount);
	void SpawnLevel(bool grid[50][50]);
	
	vector<Coordinate> findFreeSpots(Coordinate position, bool grid[50][50]);
	Coordinate exitPosition;

	UFUNCTION(BlueprintCallable, Category = "NewLevel")
	void GetNewLevel();
	UFUNCTION(BlueprintCallable, Category = "NewLevel")
	void GetOldLevel();

	//returns spread of level
	UFUNCTION(BlueprintCallable, Category = "NewLevel")
		float GetMidY();
	//returns spread of level
	UFUNCTION(BlueprintCallable, Category = "NewLevel")
		float GetMidX();

};
