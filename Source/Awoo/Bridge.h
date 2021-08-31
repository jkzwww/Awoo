// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LockedSwitch.h"
#include "FlipSwitch.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bridge.generated.h"

UCLASS()
class AWOO_API ABridge : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

	// Sets default values for this actor's properties
	ABridge();

	UPROPERTY(EditAnywhere)
		AActor* mySwitch;

	UPROPERTY(EditAnywhere)
		FVector defaultPosition;

	UPROPERTY(EditAnywhere)
		FVector targetPosition;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
