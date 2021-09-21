// Fill out your copyright notice in the Description page of Project Settings.


#include "Bridge.h"

// Sets default values
ABridge::ABridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(RootComponent);

	//set mesh
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObject(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (CubeObject.Succeeded())
	{
		VisibleComponent->SetStaticMesh(CubeObject.Object);
	}

	//default values
	isActivated = false;

}

// Called when the game starts or when spawned
void ABridge::BeginPlay()
{
	Super::BeginPlay();
	
	defaultPosition = GetActorLocation();

	AFlipSwitch* myFlip;
	ALockedSwitch* myLock;

	if (mySwitch)
	{
		myFlip = Cast<AFlipSwitch>(mySwitch);
		myLock = Cast<ALockedSwitch>(mySwitch);

		if (myLock)
		{
			myLock->ActivateEvent.AddDynamic(this, &ABridge::ActivateBridge);
		}

		if (myFlip)
		{
			myFlip->ActivateEvent.AddDynamic(this, &ABridge::ActivateBridge);
		}
	}


	//get material
	Material = VisibleComponent->GetMaterial(0);
	matInstance = VisibleComponent->CreateDynamicMaterialInstance(0, Material);
}

// Called every frame
void ABridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	FVector myEndLoc = FVector(0,0,0);

	if (isActivated)
	{
		myEndLoc = targetPosition;


		//set material parameter
		if (matInstance)
		{
			matInstance->SetScalarParameterValue("ActivationEmissive", 0.5);
		}
	}
	else
	{
		myEndLoc = defaultPosition;

		//set material parameter
		if (matInstance)
		{
			matInstance->SetScalarParameterValue("ActivationEmissive", 0);
		}
	}


	SetActorLocation(FMath::Lerp(GetActorLocation(),myEndLoc, 0.05f));
}


void ABridge::ActivateBridge()
{
	isActivated = !isActivated;
}