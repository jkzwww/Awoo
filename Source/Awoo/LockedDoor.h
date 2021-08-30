// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DoorKey.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LockedDoor.generated.h"


UCLASS()
class AWOO_API ALockedDoor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ALockedDoor();

	//Base Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pairing")
		ADoorKey* myKey;

	float baseZ; //= 330.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//door lock state
	UPROPERTY(EditAnywhere, Category = "DoorState")
		bool isLocked;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//event handler
	UFUNCTION()
		void OpenDoor();
};
