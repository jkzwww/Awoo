// Fill out your copyright notice in the Description page of Project Settings.


#include "Fuse.h"
#include "AwooCharacter.h"

AFuse::AFuse()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//default name and desc
	ItemName = FString(TEXT("Fuse"));
	ItemDesc = FString(TEXT("Some copper wires wrapped with rubbber./nMight be able to power something."));

}


// Called when the game starts or when spawned
void AFuse::BeginPlay()
{
	Super::BeginPlay();

	//get material
	Material = BaseMesh->GetMaterial(0);
	matInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);

	//set color for fuse
	if (matInstance)
	{
		matInstance->SetVectorParameterValue("ItemColor", FLinearColor(0.766667, 0.263344, 0.058910)); //orange
	}
}

// Called every frame
void AFuse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//specific function when item used by player
void AFuse::UseItem()
{
	//check if game character is near enough switch
	if (gameChar)
	{
		if (FVector::Dist(gameChar->GetActorLocation(), PowerBoxLoc) < 500)
		{
			//broadcast power event
			PowerEvent.Broadcast();

			isUsed = true;

			gameChar->MessageString = FString(TEXT("Fuse placed!"));
		}
		else
		{
			//display message
			//unusable, too far
			gameChar->MessageString = FString(TEXT("No powerbox nearby."));

			isUsed = false;
		}
	}
}