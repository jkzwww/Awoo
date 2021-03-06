// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Interactable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class AAwooCharacter;

UCLASS()
class AWOO_API AItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	//item name, description, use hint
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "info")
		FString ItemName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "info")
		FString ItemDesc;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "info")
		FString ItemHowTo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "info")
		bool isConsumable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "info")
		bool isUsed;

	AAwooCharacter* gameChar;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AActor* target);

	virtual void Interact_Implementation(AActor* target) override;

	UFUNCTION(BlueprintCallable,Category = "ItemUsage")
	virtual	void UseItem();

	UFUNCTION(BlueprintCallable,Category = "ItemUsage")
		void DropItem(FVector dropLoc);

};
