// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Engine.h"
#include "Slate.h"
#include "MenuController.generated.h"

/**
 * 
 */
class UUserWidget;

UCLASS()
class GLOWBUG_API AMenuController : public APlayerController
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<UUserWidget> GameOverWidgetType;

	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<UUserWidget> GameWonWidgetType;


public:
	void ShowGameOver();
	void HideGameOver();

	void ShowGameWon();
	void HideGameWon();

private:
	UPROPERTY(Transient)
		UUserWidget* _gameOverWidget;

	UPROPERTY(Transient)
		UUserWidget* _gameWonWidget;
	
	
};
