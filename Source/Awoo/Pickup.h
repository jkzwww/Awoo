// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.h"
#include "AwooCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class AWOO_API APickup : public AActor,public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	//mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	//pick up type
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int BoostType;

	//amount of boost effect
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		float BoostValue;

	//maximum stat value
	UPROPERTY(EditAnywhere)
		float MaxValue;

	UFUNCTION(BlueprintCallable)
		void Consume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AAwooCharacter* gameChar = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AActor* target);

	virtual void Interact_Implementation(AActor* target) override;

};
