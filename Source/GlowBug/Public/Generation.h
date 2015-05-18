// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DefaultBlock.h"
#include "ExitBlock.h"
#include "GlowBugGameMode.h"
#include <vector>
#include <ctime>
#include "Generation.generated.h"

using namespace std;

/**
 * Class to generate the level grid
 */

struct Coordinate
{
	int x;
	int y;
};

UCLASS()
class GLOWBUG_API AGeneration : public AActor
{
	GENERATED_BODY()
	AGeneration(const FObjectInitializer& ObjectInitializer);

private:
	/** Number of blocks along each side of grid */
		int32 Size;


	Coordinate GetNextPosition(vector<Coordinate> freeSpots, Coordinate currPos, int steps[4]);
	void GenerateLevel(int maxCount);

	vector<Coordinate> findFreeSpots(Coordinate position, bool grid[100][100]);
	Coordinate exitPosition;
	vector<Coordinate> getOutput(bool grid[100][100]);


	//vector<Coordinate> levelCoordinates;
	bool grid[100][100];
	int steps[4];
	

public:

	vector<Coordinate> GetNewLevel(int size);
	vector<Coordinate> GetOldLevel();

	
};
