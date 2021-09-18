// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Fuse.h"
#include "Interactable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerBox.generated.h"

class AAwooCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPowerEventDispatcher);

UCLASS()
class AWOO_API APowerBox : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerBox();

	//Base Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	//key which open this switch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pairing")
		AFuse* myFuse;
	
	UFUNCTION()
		void OnPower();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool isPowered;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//to show message when interacted
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AActor* target);

	virtual void Interact_Implementation(AActor* target) override;

	UPROPERTY(BlueprintAssignable, Category = "Activate")
		FPowerEventDispatcher PowerSwitchEvent;
};
