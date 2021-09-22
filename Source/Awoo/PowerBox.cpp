// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerBox.h"
#include "AwooCharacter.h"

// Sets default values
APowerBox::APowerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	//default
	isPowered = false;
}

// Called when the game starts or when spawned
void APowerBox::BeginPlay()
{
	Super::BeginPlay();
	
	//bind power event
	if (myFuse)
	{
		myFuse->PowerEvent.AddDynamic(this, &APowerBox::OnPower);
		myFuse->PowerBoxLoc = GetActorLocation();
	}

}

// Called every frame
void APowerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//show status when raycasted
void APowerBox::Interact_Implementation(AActor* target)
{

	AAwooCharacter* gameChar = Cast<AAwooCharacter>(target);

	if (gameChar)
	{
		if (isPowered)
		{
			gameChar->MessageString = FString(TEXT("Power ON.Switch attached unlocked."));
		}
		else
		{
			gameChar->MessageString = FString(TEXT("Power OFF.To be activated."));
		}
	}
}


void APowerBox::OnPower()
{
	isPowered = true;
	PowerSwitchEvent.Broadcast();
}