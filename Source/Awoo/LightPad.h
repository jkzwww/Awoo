// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ButtonSwitch.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightPad.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCodeEventDispatcher,bool , digit, int, index);

UCLASS()
class AWOO_API ALightPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightPad();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

	UPROPERTY(EditAnywhere)
		UMaterial* OnMaterial;

	UPROPERTY(EditAnywhere)
		UMaterial* OffMaterial;

	UPROPERTY(EditAnywhere)
		AButtonSwitch* mySwitch;

	UPROPERTY(EditAnywhere, Category = "Code")
		int myCodeIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool isOn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void ToggleLight();

	UPROPERTY(BlueprintAssignable, Category = "Code")
		FCodeEventDispatcher SendCodeEvent;
};
