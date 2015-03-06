// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowBug.h"
#include "Engine.h"
#include "RandomWalker.h"

RandomWalker::RandomWalker()
{
	posX=0;
	posY=0;
}

RandomWalker::~RandomWalker()
{
	
}

void RandomWalker::Step(TArray<FVector> locations)
{
	//randomly generate the next position
	int choice = rand()%4;
	//int choice = 0;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("choice: %f"), choice));


	switch(choice)
	{
	case 0:
		//posX+=120;
		if(locations.Contains(FVector(posX+120,posY,0.f)))
		{
			//get different position
			Step(locations);
			
		}
		else
		{
			posX+=120;
		}
		break;
	case 1:
		//posY+=120;
		if(locations.Contains(FVector(posX,posY+120,0.f)))
		{
			Step(locations);
		}
		else
		{
			posY+=120;
		}
		break;
	case 2:
		//posY-=120;
		if(locations.Contains(FVector(posX,posY+120,0.f)))
		{
			//get different position
			Step(locations);
		}
		else
		{
			posY-=120;
		}
		break;
	case 3:
		//posY-=120;
		if(locations.Contains(FVector(posX-120,posY,0.f)))
		{
			//get different position
			Step(locations);
		}
		else
		{
			posX-=120;
		}
		break;
	default:
		break;
	}

	
}

FVector RandomWalker::GetPosition()
{
	return FVector(posX,posY,0.f);
}