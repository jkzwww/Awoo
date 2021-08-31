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

}

// Called when the game starts or when spawned
void ABridge::BeginPlay()
{
	Super::BeginPlay();
	
	defaultPosition = GetActorLocation();
}

// Called every frame
void ABridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

