// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "GlowBugCharacter.h"
#include "MenuController.h"
#include "GlowBugGameMode.generated.h"

//enum to store the current state of gameplay
enum class EGlowBugPlayState : short
{
	EPlaying,
	EGameOver,
	EGameWon,
	EUnknown
};

UCLASS(minimalapi)
class AGlowBugGameMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class AMenuController> menuBP;

	AMenuController* menuC;

public:
	AGlowBugGameMode(const FObjectInitializer& ObjectInitializer);
	EGlowBugPlayState GetCurrentState() const;
	void SetCurrentState(EGlowBugPlayState NewState);



private:
	EGlowBugPlayState CurrentState;

	void HandleNewState(EGlowBugPlayState NewState);
};

FORCEINLINE EGlowBugPlayState AGlowBugGameMode::GetCurrentState() const
{
	return CurrentState;
}

