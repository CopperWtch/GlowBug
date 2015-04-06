// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "GlowBug.h"
#include "Engine.h"
#include "GlowBugGameMode.h"


AGlowBugGameMode::AGlowBugGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//menuController = GetWorld()->SpawnActor<AMenuController>(Blueprint, FVector(0,0,0), FRotator(0,0,0));

	//static ConstructorHelpers::FObjectFinder<UBlueprint> menuControllerBP(TEXT("Blueprint'/Game/Blueprints/MenuController_BP.MenuController_BP'"));
	//if (menuControllerBP.Object != NULL)
	//{
	//	menuBP = (UClass*)menuControllerBP.Object->GeneratedClass;
	//}

}
void AGlowBugGameMode::SetCurrentState(EGlowBugPlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(NewState);
}

void AGlowBugGameMode::HandleNewState(EGlowBugPlayState NewState)
{
	/*AActor* camera = Cast<ACameraActor>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	camera = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;*/

	//for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	camera = *ActorItr;
	//}
	TObjectIterator<APlayerController> pc;
	//if(!pc) return;

	switch (NewState)
	{
	case EGlowBugPlayState::EPlaying:

		/*if (!camera)
		camera = GetWorld()->SpawnActor<AActor>(FVector(2500,3500,500), FRotator(0, 0, 0));*/
		
		//if (camera && GEngine)
		//{

		//		//cam = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
		//	GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->SetViewTarget(this);

		//}

			

			
		break;
	case EGlowBugPlayState::EGameOver:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Game Over");
		if (!menuC)
		{
			menuC = GetWorld()->SpawnActor<AMenuController>(menuBP, FVector(0, 0, 0), FRotator(0, 0, 0));

		}
		if (menuC && menuBP && GetWorld())
		menuC->ShowGameOver();

		//ShowGameOver();
		break;
	case EGlowBugPlayState::EGameWon:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Game Won");
		if (!menuC)
		{
			menuC = GetWorld()->SpawnActor<AMenuController>(menuBP, FVector(0, 0, 0), FRotator(0, 0, 0));

		}
		if (menuC && menuBP && GetWorld())
			menuC->ShowGameWon();
		break;
	case EGlowBugPlayState::EUnknown:
	default:
		//do nothing
		break;
	}
}