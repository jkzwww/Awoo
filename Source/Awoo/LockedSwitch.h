// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "DoorKey.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LockedSwitch.generated.h"

UCLASS()
class AWOO_API ALockedSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockedSwitch();

	//Base Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	//z location when unlocked
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		float Zoffset;

	//key which open this switch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pairing")
		ADoorKey* myKey;

	//door lock state
	UPROPERTY(EditAnywhere, Category = "SwitchState")
		bool isLocked;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//unlock event handler
	UFUNCTION()
		void OnSwitch();

};
