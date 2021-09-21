// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "DoorKey.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LockedSwitch.generated.h"

class AAwooCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLockSwitchEventDispatcher);

UCLASS()
class AWOO_API ALockedSwitch : public AActor,public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockedSwitch();

	//Base Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	//z location when unlocked
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		float Zoffset;

	//key which open this switch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pairing")
		ADoorKey* myKey;

	//switch lock state
	UPROPERTY(EditAnywhere, Category = "SwitchState")
		bool isLocked;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//unlock event handler
	UFUNCTION()
		void OnSwitch();

	//to show message when interacted
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AActor* target);

	virtual void Interact_Implementation(AActor* target) override;

	UPROPERTY(BlueprintAssignable, Category = "Activate")
		FLockSwitchEventDispatcher ActivateEvent;

	//materials
	UPROPERTY(EditAnywhere,Category = "Material")
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;

	UPROPERTY(EditAnywhere,Category = "Material")
		FLinearColor LockColor;

};
