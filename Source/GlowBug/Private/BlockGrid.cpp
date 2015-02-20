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
	BlockSpacing = 110.f;
	Size = 5;
	countBlocks = 0;
	SetIsCompleted(false);
}

void ABlockGrid::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Hello World!"));
	
	//number of blocks
	const int32 NumBlocks = Size*Size;

	for (int32 BlockIndex = 0; BlockIndex < NumBlocks; BlockIndex++)
	{
		//Set offsets
		const float XOffset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		//Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();


		//Spawn a Block
		//const FVector BlockLocation = FVector(500.f,500.f,10.f);
		ADefaultBlock* newBlock = GetWorld()->SpawnActor<ADefaultBlock>(BlockLocation, FRotator(0, 0, 0));

		//Tell the block about its owner
		if (newBlock != NULL)
		{
			newBlock->OwningGrid = this;
			countBlocks++;
		}
		
		//create exit block
		if (BlockIndex == (NumBlocks-1))
		{
			FVector GroundOrigin;
			FVector GroundExtends;
			newBlock->GetActorBounds(false,GroundOrigin,GroundExtends);
			FVector offset = FVector(GroundExtends.X*2.f,0.0f,0.0f);
			AExitBlock* exit = GetWorld()->SpawnActor<AExitBlock>(BlockLocation + offset, FRotator(0, 0, 0));
			if (exit != NULL)
			{
				exit->OwningGrid = this;
			}
		}

		


	}
	//set Grid as completed
	SetIsCompleted(true);

	for (TActorIterator<ADefaultBlock> BlockItr(GetWorld()); BlockItr; ++BlockItr)
	{
		BlockItr->SetNeighbours();
	}
}