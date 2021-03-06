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
	isMyEventBound = false;
	isSwitchLocked = false;
}

// Called when the game starts or when spawned
void AFlipSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	defaultRotation = GetActorRotation();

	//if connected to power box
	if (myPowerbox)
	{
		//lock switch until powerbox powered
		isSwitchLocked = true;

		//bind unlock switch function to powerbox event
		myPowerbox->PowerSwitchEvent.AddDynamic(this, &AFlipSwitch::unlockSwitch);
	}

	//get material
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void AFlipSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isOn)
	{
		SetActorRotation(FRotator(defaultRotation.Pitch, defaultRotation.Yaw, -60));

		//set material parameter
		if (matInstance)
		{
			matInstance->SetScalarParameterValue("isOn", 0);
		}
	}
	else
	{
		SetActorRotation(FRotator(defaultRotation.Pitch, defaultRotation.Yaw, -120));

		//set material parameter
		if (matInstance)
		{
			matInstance->SetScalarParameterValue("isOn", 1);
		}
	}

}

//show lock status when raycasted
void AFlipSwitch::Interact_Implementation(AActor* target)
{

	gameChar = Cast<AAwooCharacter>(target);

	if (gameChar)
	{
		if (isSwitchLocked)
		{
			gameChar->MessageString = FString(TEXT("Switch locked. Connect power to powerbox."));
		}
		else
		{
			if (!isOn)
			{
				gameChar->MessageString = FString(TEXT("Switch off. Press F to flip on."));

			}
			else
			{
				gameChar->MessageString = FString(TEXT("Switch on. Press F to flip off."));

			}
		}


		//bind toggle function to event once;
		if (!isMyEventBound)
		{
			gameChar->FlipSwitchEvent.AddDynamic(this, &AFlipSwitch::ToggleSwitch);

			isMyEventBound = true;
		}
		
	}
}


//toggle switch on off
void AFlipSwitch::ToggleSwitch()
{

	if (gameChar)
	{
		//check whether switch is unlocked
		if(!isSwitchLocked)
		{
			//check whether player is near enough to switch
			if (FVector::Dist(gameChar->GetActorLocation(), GetActorLocation()) < 700)
			{
				isOn = !isOn;

				ActivateEvent.Broadcast();
			}
		}
		
	}
}
	

//switch unlock by powerbox
void AFlipSwitch::unlockSwitch()
{
	isSwitchLocked = false;
}