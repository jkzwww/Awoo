// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "Item.h"
#include "DoorKey.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKeyEventDispatcher);

UCLASS()
class AWOO_API ADoorKey : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorKey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UseItem() override;

	//event dispatcher
	UPROPERTY(BlueprintAssignable, Category = "KeyUsage")
		FKeyEventDispatcher UnlockEvent;

	//location of correspond lock/switch
	UPROPERTY(EditAnywhere)
		FVector LockLoc;

	UPROPERTY(EditAnywhere)
		float UnlockDist;
	//materials
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;

	UPROPERTY(EditAnywhere, Category = "Material")
		FLinearColor KeyColor;
};
