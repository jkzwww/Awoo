// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "AwooCharacter.h"

APickupItem::APickupItem()
{
	//all pickup can be consumed
	isConsumable = true;

	//default name and desc
	ItemName = FString(TEXT("Potion"));
	ItemDesc = FString(TEXT("Increases stats"));
	ItemHowTo = FString(TEXT("Press C to collect\nPress R to consume instantly\nPress E to equip"));

	//default values
	boostType = EPickupType::PT_HEALTH;
	boostValue = 20;

}

//specific function when item used by player
void APickupItem::UseItem()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("item Used!!"));

	if (gameChar)
	{

		isUsed = true;

		switch (boostType)
		{
		case  EPickupType::PT_HEALTH: //health type pick up
			gameChar->health += boostValue;

			if (gameChar->health > gameChar->maxHealth)
			{
				gameChar->health = gameChar->maxHealth;
			}

			gameChar->MessageString = FString(TEXT("Health increased!"));

			break;

		case EPickupType::PT_FOOD: //food type pick up
			gameChar->hunger += boostValue;

			if (gameChar->hunger > gameChar->maxHunger)
			{
				gameChar->hunger = gameChar->maxHunger;
			}

			gameChar->MessageString = FString(TEXT("Hunger increased!"));

			break;

		case EPickupType::PT_WATER:
			gameChar->hydration += boostValue;

			if (gameChar->hydration > gameChar->maxHydration)
			{
				gameChar->hydration = gameChar->maxHydration;
			}
			
			gameChar->MessageString = FString(TEXT("Hydration increased!"));

			break;

		default:

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pickup item type unspecified!!!"));
			isUsed = false;
		}

	}

}