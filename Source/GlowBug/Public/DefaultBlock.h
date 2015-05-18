/**
Game Development Project
DefaultBlock.h
Purpose: Header file for DefaultBlock.cpp

@author Sarah Bulk
*/

#pragma once

#include "GameFramework/Actor.h"
#include "DefaultBlock.generated.h"

//////////////////////////////////////////////////////////////////////////////////////
//Default Block declarations
//////////////////////////////////////////////////////////////////////////////////////
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

	//colliding actors
	TArray<AActor*> CollectedBlocks;
	
public:
	//constructor
	ADefaultBlock(const FObjectInitializer& ObjectInitializer);

	//Is this block currently active?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsActive;

	//Grid this block is spawned in
	UPROPERTY()
	class ABlockGrid* OwningGrid;

	//Collision Component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* BaseCollisionComponent;

	//Reaction to the character stepping off a block
	UFUNCTION(BlueprintNativeEvent)
	void OnSteppedOff();
	
	// Returns BaseCollisionComponent
	FORCEINLINE class USceneComponent* GetBaseCollisionComponent() const { return BaseCollisionComponent; }

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
