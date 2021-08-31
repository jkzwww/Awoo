// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipSwitch.h"

// Sets default values
AFlipSwitch::AFlipSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	//bind toggle function to player flip switch event
	AAwooCharacter* charRef = Cast<AAwooCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (charRef)
	{
		charRef->FlipSwitchEvent.AddDynamic(this, &AFlipSwitch::ToggleSwitch);
	}

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

	AAwooCharacter* gameChar = Cast<AAwooCharacter>(target);

	if (gameChar)
	{
		if (!isOn)
		{
			gameChar->MessageString = FString(TEXT("Switch off. Press F to flip on."));
			SetActorRotation(FMath::Lerp(GetActorRotation(), defaultRotation, 0.05f));
		}
		else
		{
			gameChar->MessageString = FString(TEXT("Switch on. Press F to flip off."));
			SetActorRotation(FMath::Lerp(GetActorRotation(), FRotator(defaultRotation.Pitch, -defaultRotation.Yaw, defaultRotation.Roll), 0.05f));
		}

		
	}
}


//toggle switch on off
void AFlipSwitch::ToggleSwitch()
{
	ACharacter* charRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (charRef)
	{
		//check whether player is near enough to switch
		if (FVector::Dist(charRef->GetActorLocation(), GetActorLocation()) < 500)
		{
			isOn = !isOn;
		}
		
	}
}
	