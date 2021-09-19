// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeBridge.h"

// Sets default values
ACodeBridge::ACodeBridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(RootComponent);

	//default bool
	isActivated = false;

	//3 digit code
	myCode = {0,0,1};

	//input code initialisation
	InputCode = { 0,0,0 };

}

// Called when the game starts or when spawned
void ACodeBridge::BeginPlay()
{
	Super::BeginPlay();
	
	defaultRotation = GetActorRotation();

	for (int i = 0; i < myLights.Num(); i++)
	{
		myLights[i]->SendCodeEvent.AddDynamic(this, &ACodeBridge::UpdateInput);
	}

}

// Called every frame
void ACodeBridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DigitMatched = 0;

	for (int i = 0; i < myCode.Num(); i++)
	{
		if (InputCode[i] == myCode[i])
		{
			DigitMatched++;
		}
		
	}

	if (DigitMatched == myCode.Num())
	{
		isActivated = true;
	}

	if (isActivated)
	{
		//move
		UE_LOG(LogTemp, Warning, TEXT("Code Bridge Activated!!!"));
		SetActorRotation(targetRotation);
	}
	else
	{
		SetActorRotation(defaultRotation);
	}
}


void ACodeBridge::UpdateInput(bool digit,int index)
{
	InputCode[index] = digit;
}