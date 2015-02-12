// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "GlowBug.h"
#include "GlowBugGameMode.h"
#include "GlowBugCharacter.h"

AGlowBugGameMode::AGlowBugGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


}
void AGlowBugGameMode::SetCurrentState(EGlowBugPlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(NewState);
}

void AGlowBugGameMode::HandleNewState(EGlowBugPlayState NewState)
{
	switch (NewState)
	{
	case EGlowBugPlayState::EPlaying:
		break;
	case EGlowBugPlayState::EGameOver:
		break;
	case EGlowBugPlayState::EUnknown:
	default:
		//do nothing
		break;
	}
}