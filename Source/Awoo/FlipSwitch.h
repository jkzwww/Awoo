// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "Interactable.h"
#include "AwooCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlipSwitch.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFlipSwitchEventDispatcher);

UCLASS()
class AWOO_API AFlipSwitch : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlipSwitch();

	//door lock state
	UPROPERTY(EditAnywhere, Category = "SwitchState")
		bool isOn;

	//Base Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FRotator defaultRotation;

	AAwooCharacter* gameChar;

	bool isMyEventBound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//to show message when interacted
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AActor* target);

	virtual void Interact_Implementation(AActor* target) override;

	UFUNCTION()
		void ToggleSwitch();


	UPROPERTY(BlueprintAssignable, Category = "Activate")
		FFlipSwitchEventDispatcher ActivateEvent;
};
