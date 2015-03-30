// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowBug.h"
#include "BlockGrid.h"
#include "DefaultBlock.h"
#include "ExitBlock.h"
#include "Engine.h"


ABlockGrid::ABlockGrid(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Create dummy root scene component
	DummyRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Dummy0"));
	RootComponent = DummyRoot;

	//set defaults
	countBlocks = 0;
	SetIsCompleted(false);

	Size = 50;

	steps[0] = 1;
	steps[1] = 1;
	steps[2] = 1;
	steps[3] = 1;

}

void ABlockGrid::BeginPlay()
{
	Super::BeginPlay();

	//Generate level grid
	//int c = 0;

	//do
	//{
		GenerateLevel(Size);
	//	c++;
	//} while (steps[0] > 10 || steps[1] > 10 || steps[2] > 10 || steps[3] > 10);
	//FString TheFloatStr = FString::SanitizeFloat(c);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TheFloatStr);


	SpawnLevel(grid);
	
	// USE EXITPOSITION TO SPAWN EXIT BLOCK


	//FString msg = "";
	//for(int i =0; i<Size; i++)
	//{
	//	for(int j =0; j<Size; j++)
	//	{
	//		if( grid[i][j])
	//			msg=msg+" 1";
	//		else
	//			msg=msg+" 0";
	//	}
	//	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, msg);
	//	msg="";
	//}



}

void ABlockGrid::GenerateLevel(int maxCount)
{

	Coordinate start;
	start.x=(int)maxCount/2;
	start.y=(int)maxCount/2;

	grid[start.x][start.y] = true;

	Coordinate position = start;

	//list for neighbouring positions
	vector<Coordinate> freeSpots = findFreeSpots(position,grid); 
	int count = 0;

	while(count < (int)maxCount/2 && !freeSpots.size()==0)
	{
		//Coordinate newPos = freeSpots[(int)rand() % freeSpots.size()];
		Coordinate newPos = GetNextPosition(freeSpots,position,steps);

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

		grid[position.x][position.y]=true;

		freeSpots = findFreeSpots(position,grid); 

		count++;

	}

	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, TEXT("PLACE EXIT"));
	exitPosition.x=position.x;
	exitPosition.y=position.y;

}

Coordinate ABlockGrid::GetNextPosition(vector<Coordinate> freeSpots, Coordinate currPos, int steps[4])
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

	for (int i = 0; i < freeSpots.size(); i++)
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
		int r = (int)(rand() % sum)+1;
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

vector<Coordinate> ABlockGrid::findFreeSpots(Coordinate position, bool grid[50][50])
{
	vector<Coordinate> freeSpots;

	if(!grid[position.x+1][position.y])
		{
			Coordinate ind;
			ind.x =position.x+1;
			ind.y=position.y;
			freeSpots.push_back(ind);
		}
	if(!grid[position.x-1][position.y])
	{
		Coordinate ind;
		ind.x = position.x-1;
		ind.y=position.y;
		freeSpots.push_back(ind);
	}
	if(!grid[position.x][position.y+1])
	{
		Coordinate ind;
		ind.x = position.x;
		ind.y=position.y+1;
		freeSpots.push_back(ind);
	}
	if(!grid[position.x][position.y-1])
	{
		Coordinate ind;
		ind.x = position.x;
		ind.y=position.y-1;
		freeSpots.push_back(ind);
	}

	return freeSpots;
}

void ABlockGrid::SpawnLevel(bool grid[50][50])
{

	// LOOP THROUGH FIELD[][] TO SPAWN BLOCKS FOR EVERY ELEMENT == TRUE
	for (int i = 0; i<Size; i++)
	{
		for (int j = 0; j<Size; j++)
		{
			if (grid[i][j])
			{
				Coordinate position;
				position.x = i * 120;
				position.y = j * 120;
				const FVector BlockLocation = FVector(position.x, position.y, 0.f);
				if (i == exitPosition.x && j == exitPosition.y)
				{
					AExitBlock* exit = GetWorld()->SpawnActor<AExitBlock>(BlockLocation, FRotator(0, 0, 0));
					if (exit != NULL)
					{
						exit->OwningGrid = this;
					}

				}
				else
				{
					ADefaultBlock* newBlock = GetWorld()->SpawnActor<ADefaultBlock>(BlockLocation, FRotator(0, 0, 0));
					if (newBlock != NULL)
					{
						newBlock->OwningGrid = this;
						countBlocks++;
					}
				}
			}
		}
	}

	//set Grid as completed
	SetIsCompleted(true);

	for (TActorIterator<ADefaultBlock> BlockItr(GetWorld()); BlockItr; ++BlockItr)
	{
		BlockItr->SetNeighbours();
	}

	FString TheFloatStr = FString::SanitizeFloat(steps[0]);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TheFloatStr);
	TheFloatStr = FString::SanitizeFloat(steps[1]);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TheFloatStr);
	TheFloatStr = FString::SanitizeFloat(steps[2]);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TheFloatStr);
	TheFloatStr = FString::SanitizeFloat(steps[3]);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TheFloatStr);
}