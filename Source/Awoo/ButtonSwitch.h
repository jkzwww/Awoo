// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Interactable.h"
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ButtonSwitch.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonEventDispatcher);

class AAwooCharacter;

UCLASS()
class AWOO_API AButtonSwitch : public AActor,public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AButtonSwitch();

	//Base Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Interact")
		float pushDistance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AAwooCharacter* gameChar;

	bool isMyEventBound;

	bool isPressed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintAssignable, Category = "Activate")
		FButtonEventDispatcher ToggleLightEvent;

	//to show message when interacted
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AActor* target);

	virtual void Interact_Implementation(AActor* target) override;

	UFUNCTION()
		void ButtonPressed();

	//materials
	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;

};
