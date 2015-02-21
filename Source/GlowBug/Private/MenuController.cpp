// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowBug.h"
#include "Engine.h"
#include "MenuController.h"


void AMenuController::ShowGameOver()
{
	// CreateWidget() expects the local player to be set for this player controller
	UPlayer* p = this->Player;
	auto localPlayer = Cast<ULocalPlayer>(p);

	//if (!_gameOverWidget && localPlayer)
	if (!_gameOverWidget)
	{
		_gameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetType);
	}

	if (_gameOverWidget)
	{
		_gameOverWidget->AddToViewport();
		bShowMouseCursor = true;
	}
}

void AMenuController::HideGameOver()
{
	if (_gameOverWidget)
	{
		_gameOverWidget->RemoveFromViewport();
	}
}