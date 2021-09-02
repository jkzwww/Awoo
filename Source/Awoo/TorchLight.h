// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Match.h"
#include "Interactable.h"
#include "Components/PointLightComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchLight.generated.h"

class AAwooCharacter;

UCLASS()
class AWOO_API ATorchLight : public AActor,public IInteractable
{
	GENERATED_BODY()
	
public:	

	//visible component
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

	//point light component
	UPROPERTY(VisibleAnywhere)
		class UPointLightComponent* PointLight;

	//intensity variable
	UPROPERTY(EditAnywhere)
		float LightIntensity;

	//ref to player
	AAwooCharacter* gameChar;

	// Sets default values for this actor's properties
	ATorchLight();

	bool isLit;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//toggle light function
	UFUNCTION(BlueprintCallable)
		void ToggleLight();

	//to show message when interacted
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AActor* target);

	virtual void Interact_Implementation(AActor* target) override;


};
