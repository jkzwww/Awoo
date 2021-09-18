// Fill out your copyright notice in the Description page of Project Settings.


#include "LightPad.h"

// Sets default values
ALightPad::ALightPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(RootComponent);

	OnMaterial = CreateDefaultSubobject<UMaterial>(TEXT("On Material"));
	OffMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Off Material"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshObject(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));
	ConstructorHelpers::FObjectFinder<UMaterial> OnMaterialObject(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse"));
	ConstructorHelpers::FObjectFinder<UMaterial> OffMaterialObject(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile.M_Tech_Hex_Tile"));

	if (CubeMeshObject.Succeeded())
	{
		VisibleComponent->SetStaticMesh(CubeMeshObject.Object);
	}

	if (OnMaterialObject.Succeeded())
	{
		OnMaterial = OnMaterialObject.Object;
	}

	if (OffMaterialObject.Succeeded())
	{
		OffMaterial = OffMaterialObject.Object;
	}

	//default
	isOn = false;
}

// Called when the game starts or when spawned
void ALightPad::BeginPlay()
{
	Super::BeginPlay();
	
	VisibleComponent->SetMaterial(0, OffMaterial);

	if (mySwitch)
	{
		mySwitch->ToggleLightEvent.AddDynamic(this, &ALightPad::ToggleLight);
	}
}

// Called every frame
void ALightPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isOn)
	{
		VisibleComponent->SetMaterial(0, OnMaterial);
	}
	else
	{
		VisibleComponent->SetMaterial(0, OffMaterial);
	}
}


void ALightPad::ToggleLight()
{
	isOn = !isOn;

	//broadcast to codeBridge

}

