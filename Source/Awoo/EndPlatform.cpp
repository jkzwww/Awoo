// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPlatform.h"

// Sets default values
AEndPlatform::AEndPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;
	
	//default
	RotationSpeed = 20;
}

// Called when the game starts or when spawned
void AEndPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEndPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////self rotate
	//FRotator Rotator(0, RotationSpeed * DeltaTime, 0);
	//FQuat RotationQuaternion(Rotator);

	//AddActorLocalRotation(RotationQuaternion, false, nullptr, ETeleportType::None);
}

