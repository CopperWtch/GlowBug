// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowBug.h"
#include <vector>
#include "Generation.h"

AGeneration::AGeneration(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	Size = 80;
	steps[0] = 1;
	steps[1] = 1;
	steps[2] = 1;
	steps[3] = 1;
}

vector<Coordinate> AGeneration::GetNewLevel(int size)
{
	srand(static_cast <unsigned> (time(0)));

	steps[0] = 1;
	steps[1] = 1;
	steps[2] = 1;
	steps[3] = 1;

	//Clear the grid to allow for new generation
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			grid[i][j] = false;
		}
	}


	for (TActorIterator<ADefaultBlock> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->Destroy();
	}

	for (TActorIterator<AExitBlock> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->Destroy();
	}

	GenerateLevel(size);

	return getOutput(grid);
}

vector<Coordinate> AGeneration::GetOldLevel()
{


	//level1[5][6] =
	//{
	//	{ 0, 0, 0, 1, 1, 0 }, // row 0
	//	{ 0, 0, 0, 1, 1, 0 }, // row 1
	//	{ 1, 1, 1, 0, 1, 0 }, // row 2
	//	{ 1, 1, 1, 1, 1, 0 }, // row 3
	//	{ 0, 0, 0, 0, 0, 0 } // row 3
	//};

	steps[0] = 1;
	steps[1] = 1;
	steps[2] = 1;
	steps[3] = 1;


	for (TActorIterator<ADefaultBlock> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->Destroy();
	}

	for (TActorIterator<AExitBlock> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->Destroy();
	}

	AGlowBugGameMode* gm = (AGlowBugGameMode*)GetWorld()->GetAuthGameMode();
	gm->SetCurrentState(EGlowBugPlayState::EPlaying);

	return getOutput(grid);
}

vector<Coordinate> AGeneration::getOutput(bool grid[100][100])
{
	vector<Coordinate> levelCoordinates;
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			if (grid[i][j] &&
				!(i == exitPosition.x && j == exitPosition.y))
			{
				Coordinate c;
				c.x = i * 120;
				c.y = j * 120;
				levelCoordinates.push_back(c);
			}

		}
	}

	Coordinate e;
	e.x = exitPosition.x * 120;
	e.y = exitPosition.y * 120;
	levelCoordinates.push_back(e);

	return levelCoordinates;
}

void AGeneration::GenerateLevel(int maxCount)
{
	AGlowBugGameMode* gm = (AGlowBugGameMode*)GetWorld()->GetAuthGameMode();
	gm->SetCurrentState(EGlowBugPlayState::EPlaying);

	Coordinate start;
	start.x = (int)maxCount / 2;
	start.y = (int)maxCount / 2;

	grid[start.x][start.y] = true;

	Coordinate position = start;

	//list for neighbouring positions
	vector<Coordinate> freeSpots = findFreeSpots(position, grid);
	int count = 0;

	while (count < (int)maxCount / 2 && !freeSpots.size() == 0)
	{
		//Coordinate newPos = freeSpots[(int)rand() % freeSpots.size()];
		Coordinate newPos = GetNextPosition(freeSpots, position, steps);

		//count steps for directions
		if (newPos.x == position.x)
		{
			if (newPos.y>position.y)
				steps[1]++;
			else
				steps[3]++;
		}
		else
		{
			if (newPos.x > position.x)
				steps[0]++;
			else
				steps[2]++;
		}

		position = newPos;

		grid[position.x][position.y] = true;

		freeSpots = findFreeSpots(position, grid);

		count++;

	}

	//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, TEXT("PLACE EXIT"));
	exitPosition.x = position.x;
	exitPosition.y = position.y;

}

Coordinate AGeneration::GetNextPosition(vector<Coordinate> freeSpots, Coordinate currPos, int steps[4])
{
	//calculate probability for the next steps

	float pNorth = (steps[1] + steps[2] + steps[3])*(steps[1] + steps[2] + steps[3]);
	float pEast = (steps[0] + steps[2] + steps[3])*(steps[0] + steps[2] + steps[3]);
	float pSouth = (steps[0] + steps[1] + steps[3])*(steps[0] + steps[1] + steps[3]);
	float pWest = (steps[0] + steps[1] + steps[2])*(steps[0] + steps[1] + steps[2]);
	int sum = pNorth + pEast + pSouth + pWest;

	Coordinate posNorth;
	Coordinate posEast;
	Coordinate posSouth;
	Coordinate posWest;

	for (size_t i = 0; i < freeSpots.size(); i++)
	{
		if (freeSpots[i].x == currPos.x)
		{
			if (freeSpots[i].y < currPos.y)
			{
				posWest = freeSpots[i];
			}
			else
			{
				posEast = freeSpots[i];
			}
		}
		else
		{
			if (freeSpots[i].x < currPos.x)
			{
				posSouth = freeSpots[i];
			}
			else
			{
				posNorth = freeSpots[i];
			}
		}
	}


	Coordinate nextPos;
	nextPos.x = NULL;
	nextPos.y = NULL;

	do
	{
		int r = (int)(rand() % sum) + 1;
		if (r <= pNorth)
		{
			nextPos = posNorth;
		}
		else if (r <= pNorth + pEast)
		{
			nextPos = posEast;
		}
		else if (r <= pNorth + pEast + pSouth)
		{
			nextPos = posSouth;
		}
		else
			nextPos = posWest;
	} while (nextPos.x<0);

	return nextPos;
}

vector<Coordinate> AGeneration::findFreeSpots(Coordinate position, bool grid[100][100])
{
	vector<Coordinate> freeSpots;

	if (!grid[position.x + 1][position.y])
	{
		Coordinate ind;
		ind.x = position.x + 1;
		ind.y = position.y;
		freeSpots.push_back(ind);
	}
	if (!grid[position.x - 1][position.y])
	{
		Coordinate ind;
		ind.x = position.x - 1;
		ind.y = position.y;
		freeSpots.push_back(ind);
	}
	if (!grid[position.x][position.y + 1])
	{
		Coordinate ind;
		ind.x = position.x;
		ind.y = position.y + 1;
		freeSpots.push_back(ind);
	}
	if (!grid[position.x][position.y - 1])
	{
		Coordinate ind;
		ind.x = position.x;
		ind.y = position.y - 1;
		freeSpots.push_back(ind);
	}

	return freeSpots;
}