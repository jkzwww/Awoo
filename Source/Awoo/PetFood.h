// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PetFood.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFeedEventDispatcher);

UCLASS()
class AWOO_API APetFood : public AItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APetFood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UseItem() override;

	//event dispatcher
	UPROPERTY(BlueprintAssignable)
		FFeedEventDispatcher FeedEvent;


};
