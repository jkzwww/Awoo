// Fill out your copyright notice in the Description page of Project Settings.


#include "PetFood.h"
#include "AwooCharacter.h"

APetFood::APetFood()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//default name and desc
	ItemName = FString(TEXT("Meat cube"));
	ItemDesc = FString(TEXT("Nutritious for pets."));


}


// Called when the game starts or when spawned
void APetFood::BeginPlay()
{
	Super::BeginPlay();


}


// Called every frame
void APetFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//specific function when item used by player
void APetFood::UseItem()
{
	if (gameChar)
	{
		//found pet
		if (gameChar->myPet)
		{
			FeedEvent.Broadcast();
			isUsed = true;
		}
		else
		{
			gameChar->MessageString = FString(TEXT("Not edible for human"));
			isUsed = false;
		}
		
	}
	
}
