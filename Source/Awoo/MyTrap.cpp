// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTrap.h"

// Sets default values
AMyTrap::AMyTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;

}

// Called when the game starts or when spawned
void AMyTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

