/**
Game Development Project
DefaultBlock.cpp
Purpose: Check for and process collisions of standard blocks.

@author Sarah Bulk
*/

#include "GlowBug.h"
#include "BlockGrid.h"
#include "DefaultBlock.h"
#include "Engine.h"
#include "GlowBugCharacter.h"
#include "GlowBugGameMode.h"


//////////////////////////////////////////////////////////////////////////////////////
//constructor
//////////////////////////////////////////////////////////////////////////////////////
ADefaultBlock::ADefaultBlock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//Create root scene component
	BaseCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("BaseSphereComponent"));
	BaseCollisionComponent->SetSphereRadius(65);
	RootComponent = BaseCollisionComponent;

	//Set defaults
	bIsActive = true;
	bIsColliding = false;

	//Allow Tick() to fire
	PrimaryActorTick.bCanEverTick = true;
}




//////////////////////////////////////////////////////////////////////////////////////
//React to the character stepping off a block
//////////////////////////////////////////////////////////////////////////////////////
void ADefaultBlock::OnSteppedOff_Implementation()
{
	//update block count in the owning grid
	int32 blockCount = (OwningGrid->GetCountBlocks()) - 1;
	OwningGrid->SetCountBlocks(blockCount);

	//Deactivate this block
	bIsActive = false;

	//destroy the Block to remove it from the scene
	Destroy();


	//Remove this block from its neighbours
	if (GetBlockEast())
	{
		//GetBlockEast()->SetNeighbours();
		GetBlockEast()->SetBlockWest(NULL);
	}
	if (GetBlockWest())
	{
		//GetBlockWest()->SetNeighbours();
		GetBlockWest()->SetBlockEast(NULL);
	}
	if (GetBlockNorth())
	{
		//GetBlockNorth()->SetNeighbours();
		GetBlockNorth()->SetBlockSouth(NULL);
	}
	if (GetBlockSouth())
	{
		//GetBlockSouth()->SetNeighbours();
		GetBlockSouth()->SetBlockNorth(NULL);
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//Check for collision with the character to see if they stepped off
//////////////////////////////////////////////////////////////////////////////////////
void ADefaultBlock::CheckCollision()
{
	for (TActorIterator<AGlowBugCharacter> CharacterItr(GetWorld()); CharacterItr; ++CharacterItr)
	{
			//get array of overlapping actors from character
			//describes which actors are currently colliding with the character
			TArray<AActor*> Actors = CharacterItr->CollectedActors;

			//check if THIS is currently colliding with the character
			if (!Actors.Contains(this))
			{
				//If THIS is not currently colliding with the character,
				//Check if it has up until now been colliding
				if (bIsColliding)
				{
					//If yes: Collision just ended
					//Character just stepped off the tile:
					OnSteppedOff_Implementation();
					bIsColliding = false;
				}
			}
	}

	
}

//////////////////////////////////////////////////////////////////////////////////////
//Fire on every update
//////////////////////////////////////////////////////////////////////////////////////
void ADefaultBlock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AGlowBugGameMode* gm = (AGlowBugGameMode*)GetWorld()->GetAuthGameMode();
	if (gm->GetCurrentState() == EGlowBugPlayState::EPlaying && OwningGrid != NULL && OwningGrid->IsCompleted() == true)
	{
		//call in every update to check for collision
		CheckCollision();

		//Check for Isolation & Lost game if isolated
		if (CheckIsolation())
		{
			LoseGame();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//Set the neighbouring blocks
//////////////////////////////////////////////////////////////////////////////////////
void ADefaultBlock::SetNeighbours()
{
	//get all actors type DefaultBlock that collide
	BaseCollisionComponent->GetOverlappingActors(CollectedBlocks, ADefaultBlock::StaticClass());

	//Loop through all the colliding blocks and set them as the correct neighbours
	for (int32 i = 0; i < CollectedBlocks.Num(); i++)
	{
		//Cast to DefaultBlock
		ADefaultBlock* tile = Cast<ADefaultBlock>(CollectedBlocks[i]);

		if (tile != NULL && !tile->IsPendingKill() && tile->bIsActive && tile!=this)
		{
		
		//Get positions to compare
		FVector tilePos;
		FVector tileBounds;
		tile->GetActorBounds(false, tilePos, tileBounds);

		FVector thisPos; 
		FVector thisBounds;
		this->GetActorBounds(false, thisPos, thisBounds);


		//set as neighbours depending on position compared to This
		if (tilePos.X == thisPos.X)
		{
			if (tilePos.Y>thisPos.Y)
				this->SetBlockEast(tile);
			else
				this->SetBlockWest(tile);

		}
		else if (tilePos.Y == thisPos.Y)
		{
			if (tilePos.X>thisPos.X)
				this->SetBlockNorth(tile);
			else
				this->SetBlockSouth(tile);
		}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//Check if this block is isolated
//////////////////////////////////////////////////////////////////////////////////////
bool ADefaultBlock::CheckIsolation()
{
	if (this->GetBlockSouth() == NULL && this->GetBlockNorth() == NULL && this->GetBlockEast() == NULL && this->GetBlockWest() == NULL && this->bIsActive && OwningGrid->IsCompleted()==true)
	{
		//tile has no neighbours
		return true;

	}
	else return false;

}

//////////////////////////////////////////////////////////////////////////////////////
//Methods allowing to win or lose the game
//////////////////////////////////////////////////////////////////////////////////////
void ADefaultBlock::WinGame()
{
	//get the current GameMode & change the play state
	AGlowBugGameMode* gm = (AGlowBugGameMode*)GetWorld()->GetAuthGameMode();
	gm->SetCurrentState(EGlowBugPlayState::EGameWon);

}

void ADefaultBlock::LoseGame()
{
	//get the current GameMode & change the play state
	AGlowBugGameMode* gm = (AGlowBugGameMode*)GetWorld()->GetAuthGameMode();
	gm->SetCurrentState(EGlowBugPlayState::EGameOver);

}