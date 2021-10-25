// Fill out your copyright notice in the Description page of Project Settings.


#include "Journal.h"

// Sets default values
AJournal::AJournal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	Entry = FString(TEXT("xxxxx"));
}

// Called when the game starts or when spawned
void AJournal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJournal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJournal::Interact_Implementation(AActor* target)
{
	ReadEvent.Broadcast(Entry);
}