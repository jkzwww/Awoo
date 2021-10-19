// Fill out your copyright notice in the Description page of Project Settings.


#include "Pet.h"
#include "AwooCharacter.h"

// Sets default values
APet::APet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//default values
	myFood = 0;
	skillCD = 15;
	skillTime = 7;
	speed = 2;
}

// Called when the game starts or when spawned
void APet::BeginPlay()
{
	Super::BeginPlay();
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

// Called every frame
void APet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//update current second
	currentSec = GetWorld()->UWorld::GetRealTimeSeconds();


	if (myOwner)
	{
		switch (myState)
		{
		case EPetState::PET_FOLLOW:
			toTarget = myOwner->GetActorLocation() - GetActorLocation();
			toTarget.Normalize();
			toTarget *= speed;
			BaseMesh->SetRelativeLocation(GetActorLocation() + toTarget);

			break;
		case EPetState::PET_ATTRACT:
			//broadcast charm event
			break;
		case EPetState::PET_HEAL:
			//increase owner hp
			break;
		case EPetState::PET_SHIELD:
			//add shield to owner
			break;
		default:
			break;
		}
	}

}


void APet::Interact_Implementation(AActor* target)
{

	AAwooCharacter* gameChar = Cast<AAwooCharacter>(target);

	if (gameChar)
	{
		//recognise owner if first time
		if (!myOwner)
		{
			myOwner = gameChar;
			gameChar->myPet = this;

			gameChar->MessageString = FString(TEXT("Owner, I'm so hungry and couldn't release skills."));

			myState = EPetState::PET_FOLLOW;
			myPhase = EPetPhase::PET_HUNGRY;
		}

	}
}


//when fed
void APet::getFood()
{
	
	if (myFood < 4)
	{
		myFood++;

		//update phase
		switch (myFood)
		{
		case 1:
			myPhase = EPetPhase::PET_HEALTHY;
			myOwner->MessageString = FString(TEXT("Pet : I feel better!Attraction skill unlocked!"));
			break;

		case 3:
			myPhase = EPetPhase::PET_HAPPY;
			myOwner->MessageString = FString(TEXT("Pet : Yum!!Sniff skill unlocked!"));
			break;

		case 5:
			myPhase = EPetPhase::PET_ENERGY;
			myOwner->MessageString = FString(TEXT("Pet : Awoo!!Shield skill unlocked!"));

		default:
			myOwner->MessageString = FString(TEXT("Pet : woof!"));
			break;
		}
	}
	else
	{
		myOwner->MessageString = FString(TEXT("I'm full...."));
	}

}

//when owner release skill
void APet::useSkill(int skill)
{
	//check CD
	if (currentSec - startSec < skillCD)
	{
		myOwner->MessageString = FString(TEXT("I'm still preparing..."));
	}
	else
	{
		startSec = currentSec;

		//check phase
		switch (skill)
		{
		case 1:
			if (myPhase == EPetPhase::PET_HUNGRY)
			{
				myOwner->MessageString = FString(TEXT("I'm too hungry..feed me"));
			}
			else
			{
				myState = EPetState::PET_ATTRACT;
			}

			break;

		case 2:
			if (myPhase == EPetPhase::PET_HUNGRY || myPhase == EPetPhase::PET_HEALTHY)
			{
				myOwner->MessageString = FString(TEXT("I'm still weak..feed me"));
			}
			else
			{
				myState = EPetState::PET_HEAL;
			}

			break;
		case 3:
			if (myPhase != EPetPhase::PET_ENERGY)
			{
				myOwner->MessageString = FString(TEXT("I'm still not ready..feed me"));
			}
			else
			{
				myState = EPetState::PET_SHIELD;
			}

			break;

		default:
			break;
		}
	}
	
	
}