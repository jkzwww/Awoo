// Fill out your copyright notice in the Description page of Project Settings.


#include "LockedSwitch.h"
#include "AwooCharacter.h"



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
		myKey->LockLoc = GetActorLocation();
	}

	//get material
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);

	if (matInstance)
	{
		matInstance->SetVectorParameterValue("LockColor", LockColor);
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

	if (!isLocked)
	{
		//set material parameter
		if (matInstance)
		{
			matInstance->SetScalarParameterValue("isUnlock", 1);
		}
	}
}

void ALockedSwitch::OnSwitch()
{
	isLocked = false;
	ActivateEvent.Broadcast();
}



//show lock status when raycasted
void ALockedSwitch::Interact_Implementation(AActor* target)
{

	AAwooCharacter* gameChar = Cast<AAwooCharacter>(target);

	if (gameChar)
	{
		if (isLocked)
		{
			gameChar->MessageString = FString(TEXT("Switch locked. Find and use key to unlock."));
		}
		else
		{
			gameChar->MessageString = FString(TEXT("Switch unlocked."));
		}
	}
}