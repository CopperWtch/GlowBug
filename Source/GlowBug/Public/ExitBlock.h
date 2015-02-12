// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DefaultBlock.h"
#include "ExitBlock.generated.h"

/**
 * 
 */
UCLASS()
class GLOWBUG_API AExitBlock : public ADefaultBlock
{
	GENERATED_BODY()
	
public:
	AExitBlock(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnSteppedOn();
	
};
