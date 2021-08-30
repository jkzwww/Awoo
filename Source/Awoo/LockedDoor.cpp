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

}

// Called when the game starts or when spawned
void ALockedDoor::BeginPlay()
{
	Super::BeginPlay();

	if (myKey)
	{
		myKey->UnlockEvent.AddDynamic(this, &ALockedDoor::OpenDoor);
	}

	baseZ = BaseMesh->GetComponentLocation().Z;

}

// Called every frame
void ALockedDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FVector target = BaseMesh->GetComponentLocation();

	if (!isLocked)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Door Open!!"));
		target.Z = baseZ + 350.0f;
	}
	else
	{
		target.Z = baseZ;
	}

	BaseMesh->SetRelativeLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), target, 0.05f));

}


void ALockedDoor::OpenDoor()
{
	isLocked = false;
}