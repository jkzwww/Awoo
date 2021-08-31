// Fill out your copyright notice in the Description page of Project Settings.


#include "PitTrap.h"

APitTrap::APitTrap()
{
	//bind overlap event
	OnActorBeginOverlap.AddDynamic(this, &APitTrap::OnOverlapBegin);
}


void APitTrap::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale() * 100, FColor::Turquoise, true, -1, 0, 5);
}


void APitTrap::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{

	if (OtherActor && (OtherActor != this))
	{
		AAwooCharacter* gameChar = Cast<AAwooCharacter>(OtherActor);

		if (gameChar)
		{
			//call kill function which broadcast game over event
			gameChar->killMe();
		}

	}

}