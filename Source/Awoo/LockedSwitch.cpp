// Fill out your copyright notice in the Description page of Project Settings.


#include "LockedSwitch.h"

// Sets default values
ALockedSwitch::ALockedSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	//default values
	isLocked = true;
}

// Called when the game starts or when spawned
void ALockedSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	if (myKey)
	{
		myKey->UnlockEvent.AddDynamic(this, &ALockedSwitch::OnSwitch);
	}
}

// Called every frame
void ALockedSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (!isLocked)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Switch Open!!"));
	//}
}

void ALockedSwitch::OnSwitch()
{
	isLocked = false;
}

void ALockedSwitch::Interact_Implementation(AActor* target)
{
	AAwooCharacter* gameChar = Cast<AAwooCharacter>(target);

	//display lock status when player is near
	if (gameChar)
	{
		if (isLocked)
		{
			gameChar->MessageString = FString(TEXT("Switch LOCKED"));
		}
		else
		{
			gameChar->MessageString = FString(TEXT("Switch UNLOCKED"));
		}
		
	}
}