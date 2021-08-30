// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PickupItem.generated.h"

/**
 * 
 */
UCLASS()
class AWOO_API APickupItem : public AItem
{
	GENERATED_BODY()
	
public:
	APickupItem();

	virtual void UseItem() override;

	//amount of stats to boost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		float boostValue;

	//type of stat to boost
	// 0: health, 1: food, 2: water
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		int boostType;
};
