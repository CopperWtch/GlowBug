// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowBug.h"
#include "ExitBlock.h"
#include "Engine.h"
#include "GlowBugGameMode.h"
#include "BlockGrid.h"



AExitBlock::AExitBlock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//replace material
	//static ConstructorHelpers::FObjectFinder<UMaterial> Material_Blue(TEXT("MaterialInstanceConstant'/Game/StarterContent/Materials/M_Rock_Sandstone.M_Rock_Sandstone'"));
	//BlockMesh->SetMaterial(0, Material_Blue.Object);


}

void AExitBlock::OnSteppedOn_Implementation()
{
	AGlowBugGameMode* gm = (AGlowBugGameMode*)GetWorld()->GetAuthGameMode();

	//There is at least 1 Block remaining in the level. 
	//The Player loses the game
	if (OwningGrid->GetCountBlocks() > 1)
	{
		this->LoseGame();
	}
	//There are no blocks left in the level except for the exit tile.
	//The player wins
	else{
		this->WinGame();
	}

}

void AExitBlock::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//call in every update to check for collision
	AGlowBugGameMode* gm = (AGlowBugGameMode*)GetWorld()->GetAuthGameMode();

	if (bIsColliding&&gm->GetCurrentState()==EGlowBugPlayState::EPlaying)
	{
		OnSteppedOn_Implementation();
	}
		
}

bool AExitBlock::CheckIsolation()
{
	//FString TheFloatStr = FString::SanitizeFloat(this->GetActorLocation().X);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "TOAST");
	return false;

}
