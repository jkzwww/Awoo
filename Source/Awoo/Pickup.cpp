// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;

	//default values
	BoostType = 0;
	BoostValue = 15;
	MaxValue = 100;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APickup::Interact_Implementation(AActor* target)
{
	//debug message
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Picked Up"));

	//check if picked up by character
	gameChar = Cast<AAwooCharacter>(target);

	if (gameChar)
	{
		gameChar->myPickups.Add(this);

		switch (BoostType)
		{
		case 0: gameChar->myMeds++;
			break;

		case 1: gameChar->myFood++;
			break;

		case 2:gameChar->myDrinks++;
			break;

		default: break;
		}
	}

	//disappear as picked up
	// Hides visible components
	SetActorHiddenInGame(true);

	// Disables collision components
	SetActorEnableCollision(false);

}


void APickup::Consume()
{
	switch (BoostType)
	{
	case 0://med
		//increase character health if not full
		gameChar->health += BoostValue;
		gameChar->myMeds--;
		if (gameChar->health > 100) { gameChar->health = 100; }

		//gameChar->MessageString = FString(TEXT("Health boosted!!"));

		break;

	case 1://food
		//increase character hunger if not full
		gameChar->hunger += BoostValue;
		gameChar->myFood--;
		if (gameChar->hunger > 100) { gameChar->hunger = 100; }

		//gameChar->MessageString = FString(TEXT("Yum yum!!"));

		break;
	case 2://water
		//increase character hydration if not full
		gameChar->hydration += BoostValue;
		gameChar->myDrinks--;
		if (gameChar->hydration > 100) { gameChar->hydration = 100; }

		//gameChar->MessageString = FString(TEXT("Hydration UP!!"));

		break;
	default:
		//wrong type
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pick Up Type Unspecified"));
	}

	//sound and particle effects
	
	Destroy();
}