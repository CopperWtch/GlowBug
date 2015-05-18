// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowBug.h"
#include "BlockGrid.h"
#include "DefaultBlock.h"
#include "Engine.h"
#include "GlowBugCharacter.h"
#include "GlowBugGameMode.h"




//constructor
ADefaultBlock::ADefaultBlock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{



	//Create root scene component
	BaseCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("BaseSphereComponent"));
	BaseCollisionComponent->SetSphereRadius(65);
	RootComponent = BaseCollisionComponent;
	

	//Create the static mesh component
	/*BlockMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BlockMesh"));*/

	//create mesh and material
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	//
	//static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("MaterialInstanceConstant'/Game/StarterContent/Materials/M_AssetPlatform.M_AssetPlatform'"));
	//if (Material.Object != NULL)
	//{
	//	UMaterialInstance* TheMaterial = (UMaterialInstance*)Material.Object;
	//	UMaterialInstanceDynamic* TheMaterial_Dyn = UMaterialInstanceDynamic::Create(TheMaterial, this);
	//	BlockMesh->SetMaterial(0, TheMaterial_Dyn);
	//}

	//BlockMesh->SetStaticMesh(StaticMesh.Object);
	//BlockMesh->SetRelativeScale3D(FVector(1, 1, 0.25f));
	//BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	//attach the static mesh component to the root component
	/*BlockMesh->AttachTo(RootComponent);*/


	bIsActive = true;
	bIsColliding = false;

	//Tick() function fires:
	PrimaryActorTick.bCanEverTick = true;
}




//methods
void ADefaultBlock::OnSteppedOff_Implementation()
{
	int32 blockCount = (OwningGrid->GetCountBlocks()) - 1;
	OwningGrid->SetCountBlocks(blockCount);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "" + blockCount);

	//Block is no longer active
	bIsActive = false;

	//destroy the Block to remove it from the scene
	Destroy();


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

void ADefaultBlock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AGlowBugGameMode* gm = (AGlowBugGameMode*)GetWorld()->GetAuthGameMode();
	if (gm->GetCurrentState() == EGlowBugPlayState::EPlaying && OwningGrid != NULL && OwningGrid->IsCompleted() == true)
	{
		//call in every update to check for collision
		CheckCollision();

		//Check for Isolation
		if (CheckIsolation())
		{
			LoseGame();
		}
	}
}

void ADefaultBlock::SetNeighbours()
{
	//get all actors type DefaultBlock that collide
	BaseCollisionComponent->GetOverlappingActors(CollectedBlocks, ADefaultBlock::StaticClass());

	for (int32 i = 0; i < CollectedBlocks.Num(); i++)
	{
		//Cast to DefaultBlock
		ADefaultBlock* tile = Cast<ADefaultBlock>(CollectedBlocks[i]);

		if (tile != NULL && !tile->IsPendingKill() && tile->bIsActive && tile!=this)
		{
		
		//Get positions to compare
		FVector tilePos;//= CollectedTiles[i]->GetActorLocation();
		FVector tileBounds;
		tile->GetActorBounds(false, tilePos, tileBounds);

		FVector thisPos; //= this->GetActorLocation();
		FVector thisBounds;
		this->GetActorBounds(false, thisPos, thisBounds);

		//debug
		//FString TheFloatStr = FString::SanitizeFloat(tilePos.X);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *TheFloatStr);

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

bool ADefaultBlock::CheckIsolation()
{
	if (this->GetBlockSouth() == NULL && this->GetBlockNorth() == NULL && this->GetBlockEast() == NULL && this->GetBlockWest() == NULL && this->bIsActive && OwningGrid->IsCompleted()==true)
	{
		//tile has no neighbours
		//ISolation: 

		/*FString TheFloatStr = FString::SanitizeFloat(this->GetActorLocation().X);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *TheFloatStr);*/

		return true;

	}
	else return false;

}

void ADefaultBlock::WinGame()
{
	AGlowBugGameMode* gm = (AGlowBugGameMode*)GetWorld()->GetAuthGameMode();
	gm->SetCurrentState(EGlowBugPlayState::EGameWon);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "WIN!");
}

void ADefaultBlock::LoseGame()
{
	AGlowBugGameMode* gm = (AGlowBugGameMode*)GetWorld()->GetAuthGameMode();
	gm->SetCurrentState(EGlowBugPlayState::EGameOver);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "LOST!");
}