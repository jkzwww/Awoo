// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipSwitch.h"

// Sets default values
AFlipSwitch::AFlipSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;


	//default values
	isOn = false;
}

// Called when the game starts or when spawned
void AFlipSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	defaultRotation = GetActorRotation();
}

// Called every frame
void AFlipSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//show lock status when raycasted
void AFlipSwitch::Interact_Implementation(AActor* target)
{

	gameChar = Cast<AAwooCharacter>(target);

	if (gameChar)
	{
		if (!isOn)
		{
			gameChar->MessageString = FString(TEXT("Switch off. Press F to flip on."));
			
		}
		else
		{
			gameChar->MessageString = FString(TEXT("Switch on. Press F to flip off."));
			
		}

		//bind toggle function to event
		gameChar->FlipSwitchEvent.AddDynamic(this, &AFlipSwitch::ToggleSwitch);
	}
}


//toggle switch on off
void AFlipSwitch::ToggleSwitch()
{

	if (gameChar)
	{
		//check whether player is near enough to switch
		if (FVector::Dist(gameChar->GetActorLocation(), GetActorLocation()) < 500)
		{
			isOn = !isOn;
			SetActorRotation(FRotator(defaultRotation.Pitch, -1 * (defaultRotation.Yaw), defaultRotation.Roll));
		}
		
	}
}
	