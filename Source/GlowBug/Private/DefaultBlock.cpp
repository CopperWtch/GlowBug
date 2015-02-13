// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowBug.h"
#include "BlockGrid.h"
#include "DefaultBlock.h"
#include "Engine.h"
#include "GlowBugCharacter.h"

//constructor
ADefaultBlock::ADefaultBlock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{



	//Create root scene component
	BaseCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("BaseSphereComponent"));
	RootComponent = BaseCollisionComponent;
	

	//Create the static mesh component
	BlockMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BlockMesh"));

	//create mesh and material
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> Material_Blue(TEXT("MaterialInstanceConstant'/Game/StarterContent/Materials/M_AssetPlatform.M_AssetPlatform'"));
	
	BlockMesh->SetStaticMesh(StaticMesh.Object);
	BlockMesh->SetMaterial(0, Material_Blue.Object);
	BlockMesh->SetRelativeScale3D(FVector(1, 1, 0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	//attach the static mesh component to the root component
	BlockMesh->AttachTo(RootComponent);


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
	//To Debug we print a log on the screen
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Tile Destroyed!");
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
	//call in every update to check for collision
	CheckCollision();
}