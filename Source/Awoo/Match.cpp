// Fill out your copyright notice in the Description page of Project Settings.


#include "Match.h"

AMatch::AMatch()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//default name and desc
	ItemName = FString(TEXT("Match"));
	ItemDesc = FString(TEXT("A small match box with a few matches left."));
	//MatchNum = 5;

}


void AMatch::UseItem()
{
	LightUpEvent.Broadcast();

	isUsed = true;

	//MatchNum--;

	//if (MatchNum == 0)
	//{
	//	isUsed = true;
	//}
}