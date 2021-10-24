// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonSwitch.h"
#include "AwooCharacter.h"

// Sets default values
AButtonSwitch::AButtonSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	//default values
	isMyEventBound = false;
	isPressed = false;
	pushDistance = 400;
}

// Called when the game starts or when spawned
void AButtonSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	//get material
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);


}

// Called every frame
void AButtonSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isPressed)
	{

		if (matInstance)
		{
			matInstance->SetScalarParameterValue("ButtonOn", 1);
		}
	}
	else
	{

		if (matInstance)
		{
			matInstance->SetScalarParameterValue("ButtonOn", 0);
		}
	}


}


void AButtonSwitch::ButtonPressed()
{
	if (gameChar)
	{
		FVector TargetDirection = gameChar->GetActorLocation() - GetActorLocation();


		//check whether player is near enough to switch
		if (TargetDirection.Size() < pushDistance)
		{
			TargetDirection.Normalize();
			float DotProduct = FVector::DotProduct(GetActorForwardVector(), TargetDirection);
		
			if(DotProduct > 0.9) //only light up when character is in front
			{
				ToggleLightEvent.Broadcast();
				UE_LOG(LogTemp, Warning, TEXT("Button pressed"));
				isPressed = !isPressed;
			}
		
		}
	}
	
}


//show button status when raycasted
void AButtonSwitch::Interact_Implementation(AActor* target)
{

	gameChar = Cast<AAwooCharacter>(target);

	if (gameChar)
	{

		gameChar->MessageString = FString(TEXT("Hit F to press or release button."));

	}

	//bind toggle function to event once;
	if (!isMyEventBound)
	{
		gameChar->FlipSwitchEvent.AddDynamic(this, &AButtonSwitch::ButtonPressed);

		isMyEventBound = true;
	}
}
