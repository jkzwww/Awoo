// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LightPad.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CodeBridge.generated.h"

UCLASS()
class AWOO_API ACodeBridge : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACodeBridge();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

	UPROPERTY(EditAnywhere, Category = "Code")
		TArray<bool> myCode;

	UPROPERTY(EditAnywhere, Category = "Code")
		TArray<bool> InputCode;

	UPROPERTY(EditAnywhere, Category = "Code")
		TArray<ALightPad*> myLights;

	UPROPERTY(EditAnywhere)
		FRotator defaultRotation;

	UPROPERTY(EditAnywhere)
		FRotator targetRotation;
	

	UFUNCTION()
		void UpdateInput(bool digit,int index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool isActivated;

	int DigitMatched;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
