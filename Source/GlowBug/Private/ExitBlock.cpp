// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowBug.h"
#include "ExitBlock.h"

AExitBlock::AExitBlock(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//replace material
	static ConstructorHelpers::FObjectFinder<UMaterial> Material_Blue(TEXT("MaterialInstanceConstant'/Game/StarterContent/Materials/M_Rock_Sandstone.M_Rock_Sandstone'"));
	BlockMesh->SetMaterial(0, Material_Blue.Object);
}

void AExitBlock::OnSteppedOn_Implementation()
{
	//TODO
	//WIN
}


