// Fill out your copyright notice in the Description page of Project Settings.


#include "Fuse.h"

AFuse::AFuse()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//default name and desc
	ItemName = FString(TEXT("Fuse"));
	ItemDesc = FString(TEXT("Some copper wires wrapped with rubbber./nMight be able to power something."));


}


// Called when the game starts or when spawned
void AFuse::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFuse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//specific function when item used by player
void AFuse::UseItem()
{

}