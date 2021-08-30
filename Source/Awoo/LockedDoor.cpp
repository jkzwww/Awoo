// Fill out your copyright notice in the Description page of Project Settings.


#include "LockedDoor.h"

// Sets default values
ALockedDoor::ALockedDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	//default values
	isLocked = true;

	//bind the open door event to my paired key
	if (myKey)
	{
		myKey->UnlockEvent.AddDynamic(this, &ALockedDoor::OpenDoor);
	}

}

// Called when the game starts or when spawned
void ALockedDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALockedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isLocked)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Door Open!!"));


	}
}


void ALockedDoor::OpenDoor()
{
	isLocked = false;
}