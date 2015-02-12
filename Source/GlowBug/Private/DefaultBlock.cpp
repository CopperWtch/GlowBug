// Fill out your copyright notice in the Description page of Project Settings.

#include "GlowBug.h"
#include "DefaultBlock.h"

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
	BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));

	//attach the static mesh component to the root component
	BlockMesh->AttachTo(RootComponent);


	bIsActive = true;

}

//methods
void ADefaultBlock::OnSteppedOff_Implementation()
{
	//TODO
	//Destroy();
}