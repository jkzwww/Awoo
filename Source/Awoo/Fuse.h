// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Fuse.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuseEventDispatcher);

UCLASS()
class AWOO_API AFuse : public AItem
{
	GENERATED_BODY()
	
public:
	AFuse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UseItem() override;

	//location of correspond powerbox
	UPROPERTY(EditAnywhere)
		FVector PowerBoxLoc;

	//event dispatcher
	UPROPERTY(BlueprintAssignable, Category = "Usage")
		FFuseEventDispatcher PowerEvent;


	//materials
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;
};
