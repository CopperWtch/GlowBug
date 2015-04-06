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
private:

	//neighbour blocks
	ADefaultBlock* blockNorth;
	ADefaultBlock* blockSouth;
	ADefaultBlock* blockEast;
	ADefaultBlock* blockWest;

	TArray<AActor*> CollectedBlocks;
	
public:
	ADefaultBlock(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsActive;

	/** Grid that owns us */
	UPROPERTY()
	class ABlockGrid* OwningGrid;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* BaseCollisionComponent;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	//class UStaticMeshComponent* BlockMesh;

	UFUNCTION(BlueprintNativeEvent)
		void OnSteppedOff();
	
	/** Returns BaseCollisionComponent **/
	FORCEINLINE class USceneComponent* GetBaseCollisionComponent() const { return BaseCollisionComponent; }
	/** Returns BlockMesh subobject **/
	//FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }


	//for collision
	bool bIsColliding;
	virtual void Tick(float DeltaSeconds) OVERRIDE;
	void CheckCollision();


	//neighbour tiles GETTER
	ADefaultBlock* GetBlockSouth(){ return blockSouth; };
	ADefaultBlock* GetBlockNorth(){ return blockNorth; };
	ADefaultBlock* GetBlockWest(){ return blockWest; };
	ADefaultBlock* GetBlockEast(){ return blockEast; };

	//neighbour tiles SETTER
	void SetBlockSouth(ADefaultBlock* tile){ blockSouth = tile; };
	void SetBlockNorth(ADefaultBlock* tile){ blockNorth = tile; };
	void SetBlockWest(ADefaultBlock* tile){ blockWest = tile; };
	void SetBlockEast(ADefaultBlock* tile){ blockEast = tile; };

	//sets neighbour blocks for a block
	void SetNeighbours();

	//check if this block is isolated
	virtual bool CheckIsolation();

	//call when the player won
	void WinGame();
	//call when the player lost
	void LoseGame();


};
