// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DefaultBlock.generated.h"

/**
 * 
 */
UCLASS()
class GLOWBUG_API ADefaultBlock : public AActor
{
	GENERATED_BODY()
	
public:
	ADefaultBlock(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsActive;

	/** Grid that owns us */
	UPROPERTY()
	class ABlockGrid* OwningGrid;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* BaseCollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* BlockMesh;

	UFUNCTION(BlueprintNativeEvent)
		void OnSteppedOff();
	
	/** Returns BaseCollisionComponent **/
	FORCEINLINE class USceneComponent* GetBaseCollisionComponent() const { return BaseCollisionComponent; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
};
