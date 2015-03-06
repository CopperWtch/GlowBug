// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
 //    USTRUCT()
 //    struct FPos
 //    {
 //        GENERATED_USTRUCT_BODY()// underlined with "this declaration has no storage class or type specifier"
     
 //        UPROPERTY()
 //            float posX; 
 //        UPROPERTY()
 //            float posY;
     
 //        FPos(){
 //        }
 //    };

class GLOWBUG_API RandomWalker
{
public:
	RandomWalker();
	~RandomWalker();

	void Step(TArray<FVector>);

private:
	int32 posX;
	int32 posY;
public:
	FVector GetPosition();
};
