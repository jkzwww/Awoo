// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AwooCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "PitTrap.generated.h"

/**
 * 
 */
UCLASS()
class AWOO_API APitTrap : public ATriggerVolume
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	APitTrap();

	// overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
