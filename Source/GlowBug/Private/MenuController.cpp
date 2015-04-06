// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowBug.h"
#include "Engine.h"
#include "MenuController.h"


void AMenuController::ShowGameOver()
{
	// CreateWidget() expects the local player to be set for this player controller
	UPlayer* p = this->Player;
	auto localPlayer = Cast<ULocalPlayer>(p);

	//if (!_gameOverWidget)
	if (!_gameOverWidget)
	{
		_gameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetType);
	}

	if (_gameOverWidget)
	{
		_gameOverWidget->AddToViewport();
		APlayerController* MyController = GetWorld()->GetFirstPlayerController();

		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
	}

}

void AMenuController::ShowGameWon()
{
	// CreateWidget() expects the local player to be set for this player controller
	UPlayer* p = this->Player;
	auto localPlayer = Cast<ULocalPlayer>(p);

	//if (!_gameOverWidget)
	if (!_gameWonWidget)
	{
		_gameWonWidget = CreateWidget<UUserWidget>(GetWorld(), GameWonWidgetType);
	}

	if (_gameWonWidget)
	{
		_gameWonWidget->AddToViewport();
		APlayerController* MyController = GetWorld()->GetFirstPlayerController();

		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
	}

}

void AMenuController::HideGameOver()
{
	if (_gameOverWidget)
	{
		_gameOverWidget->RemoveFromViewport();
	}
}

void AMenuController::HideGameWon()
{
	if (_gameWonWidget)
	{
		_gameWonWidget->RemoveFromViewport();
	}
}