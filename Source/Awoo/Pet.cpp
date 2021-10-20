// Fill out your copyright notice in the Description page of Project Settings.


#include "Pet.h"
#include "AwooCharacter.h"
#include"PetFood.h"

// Sets default values
APet::APet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	//default values
	myFood = 0;
	skillCD = 15;
	skillTime = 7;
	speed = 2;
	followDist = 30;
	healRate = 0.2;
	startSec = 0;
}

// Called when the game starts or when spawned
void APet::BeginPlay()
{
	Super::BeginPlay();
	
	//bind get food function to pet food items
	TArray<AActor*>foundFood;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APetFood::StaticClass(), foundFood);

	for (AActor* foodRef : foundFood)
	{
		APetFood* tempFood = Cast<APetFood>(foodRef);
		
		if (tempFood)
		{
			tempFood->FeedEvent.AddDynamic(this, &APet::getFood);
		}
	}
	
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

			//if not too close , follow up
			if (FVector::Dist(myOwner->GetActorLocation(), GetActorLocation()) > followDist)
			{
				toTarget = myOwner->GetActorLocation() - GetActorLocation();
				toTarget.Normalize();
				toTarget *= speed;
				BaseMesh->SetRelativeLocation(GetActorLocation() + toTarget);
			}
			break;


		case EPetState::PET_ATTRACT:
			
			
			if (currentSec - startSec < skillTime)
			{

				//get further from owner and attract enemies
				if (FVector::Dist(myOwner->GetActorLocation(), GetActorLocation()) > charmDist)
				{
					toTarget = myOwner->GetActorLocation() - GetActorLocation();
					toTarget.Normalize();
					toTarget *= speed;
					BaseMesh->SetRelativeLocation(GetActorLocation() + toTarget);
				}

				//stay and start attracting enemies
				CharmEvent.Broadcast(true,GetActorLocation());
			}
			else
			{
				//stop charming back to follow after skill
				CharmEvent.Broadcast(false,GetActorLocation());

				myState = EPetState::PET_FOLLOW;
			}
		
			break;

		case EPetState::PET_HEAL:
			//increase owner hp

			if (currentSec - startSec < skillTime)
			{
				if ((myOwner->health + healRate) < myOwner->maxHealth)
				{
					myOwner->health += healRate;

				}
			}
			else
			{
				myState = EPetState::PET_FOLLOW;
			}

			//heal effect
			
			break;


		case EPetState::PET_SHIELD:
			//add shield to owner
			if (currentSec - startSec < skillTime)
			{
				myOwner->isProtected = true;
			}
			else
			{
				myOwner->isProtected = false;
				myState = EPetState::PET_FOLLOW;
			}

			//shield effect
			
			break;
		
		default:
			break;
		}
	}

	//update HUD
	if (currentSec - startSec > skillCD)
	{

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

			//bind skill events
			myOwner->PetSkillEvent.AddDynamic(this, &APet::useSkill);
		}
		else
		{
			gameChar->MessageString = FString(TEXT(""));
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
			myOwner->MessageString = FString(TEXT("Pet : I feel better!Charm skill unlocked!"));

			//change material param

			break;

		case 3:
			myPhase = EPetPhase::PET_HAPPY;
			myOwner->MessageString = FString(TEXT("Pet : Yum!!Healing skill unlocked!"));

			//change material param

			break;

		case 5:
			myPhase = EPetPhase::PET_ENERGY;
			myOwner->MessageString = FString(TEXT("Pet : Awoo!!Shield skill unlocked!"));

			//change material param

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
	if (currentSec - startSec < skillCD && startSec > 0.01)
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
				myOwner->MessageString = FString(TEXT("They'll come for me when you're seen now!"));
	
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
				myOwner->MessageString = FString(TEXT("Don't worry, I'll give you my power!"));
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
				myOwner->MessageString = FString(TEXT("Go ahead,you're under my shield!"));
			}

			break;

		default:
			break;
		}
	}
	
	
}