// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorKey.h"
#include "AwooCharacter.h"

// Sets default values
ADoorKey::ADoorKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//default name and desc
	ItemName = FString(TEXT("Key"));
	ItemDesc = FString(TEXT("An old and rusty key\nmight be useful for a lock somewhere"));


}

// Called when the game starts or when spawned
void ADoorKey::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADoorKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//specific function when item used by player
void ADoorKey::UseItem()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Key Used!!"));

	//check if game character is near enough switch
	if (gameChar)
	{
		if (FVector::Dist(gameChar->GetActorLocation(), LockLoc) < 80)
		{
			//call key event binded to unlock event of door
			UnlockEvent.Broadcast();

			isUsed = true;

			gameChar->MessageString = FString(TEXT("Switch UNLOCKED!"));
		}
		else
		{
			//display message
			//unusable, too far
			gameChar->MessageString = FString(TEXT("Too far from lock."));

			isUsed = false;
		}
	}
	

}