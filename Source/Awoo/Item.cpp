// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	//test values
	ItemName = FString(TEXT("Test product"));
	ItemDesc = FString(TEXT("Nothing useful"));
	ItemHowTo = FString(TEXT("Press C to collect"));
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::Interact_Implementation(AActor* target)
{

	



}


void AItem::CollectItem()
{
	// Hides visible components
	SetActorHiddenInGame(true);

	// Disables collision components
	SetActorEnableCollision(false);

	// Stops the Actor from ticking
	//SetActorTickEnabled(false);
}


void AItem::UseItem()
{
	//to be override

	//broadcast item event eg.unlock 

	
}


void AItem::ThrowItem()
{
	Destroy();
}