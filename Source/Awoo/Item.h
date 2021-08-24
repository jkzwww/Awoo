// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.h"
#include "AwooCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FItemEventDispatcher, FString, name, FString, desc, FString, howTo);

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
	UPROPERTY(EditAnywhere,Category = "info")
		FString ItemName;

	UPROPERTY(EditAnywhere, Category = "info")
		FString ItemDesc;
	
	UPROPERTY(EditAnywhere, Category = "info")
		FString ItemHowTo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AActor* target);

	virtual void Interact_Implementation(AActor* target) override;

	UPROPERTY(BlueprintAssignable, Category = "Display")
		FItemEventDispatcher ShowInfoEvent;

	UFUNCTION(BlueprintCallable, Category = "ItemUsage")
		void CollectItem();

	UFUNCTION(BlueprintCallable,Category = "ItemUsage")
		void UseItem();

	UFUNCTION(BlueprintCallable,Category = "ItemUsage")
		void ThrowItem();
};
