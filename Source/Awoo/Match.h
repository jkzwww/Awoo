// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Match.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchEventDispatcher);

UCLASS()
class AWOO_API AMatch : public AItem
{
	GENERATED_BODY()

public:

	AMatch();

	virtual void UseItem() override;

	//event dispatcher
	UPROPERTY(BlueprintAssignable, Category = "Torchlight")
		FMatchEventDispatcher LightUpEvent;

	//UPROPERTY(EditAnywhere)
	//	int MatchNum;

};
