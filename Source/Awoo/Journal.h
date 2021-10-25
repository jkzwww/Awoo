// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Journal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNoteEventDispatcher,FString,myEntry);

UCLASS()
class AWOO_API AJournal : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJournal();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Entry;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AActor* target);

	virtual void Interact_Implementation(AActor* target) override;

	//event dispatcher
	UPROPERTY(BlueprintAssignable)
		FNoteEventDispatcher ReadEvent;

};
