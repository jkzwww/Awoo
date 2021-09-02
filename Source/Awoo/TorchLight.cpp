// Fill out your copyright notice in the Description page of Project Settings.


#include "TorchLight.h"
#include "AwooCharacter.h"


// Sets default values
ATorchLight::ATorchLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	PointLight->Intensity = 0;
	RootComponent = PointLight;


	//private property unaccessible
	//PointLight->bVisible = false;
	//use intensity instead

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	//VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	//VisibleComponent->SetupAttachment(RootComponent);

	//default intensity
	LightIntensity = 300.0f;
	isLit = false;
	

}

// Called when the game starts or when spawned
void ATorchLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATorchLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATorchLight::ToggleLight()
{
	
	if (FVector::Dist(gameChar->GetActorLocation(),GetActorLocation()) < 500)
	{
		PointLight->Intensity = LightIntensity;
		gameChar->MessageString = FString(TEXT(""));
	}
	else
	{
		gameChar->MessageString = FString(TEXT("Too far from torch"));
	}
}


void ATorchLight::Interact_Implementation(AActor* target)
{

	gameChar = Cast<AAwooCharacter>(target);

	if (gameChar)
	{
		gameChar->MessageString = FString(TEXT("Light me up!"));
	}


}