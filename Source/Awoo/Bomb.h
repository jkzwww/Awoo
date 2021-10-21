// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

class AEnemyCharacter;

UCLASS()
class AWOO_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomb();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

	//effects
	UPROPERTY(EditAnywhere)
		UParticleSystem* BombParticle;

	UPROPERTY(EditAnywhere)
		USoundBase* ExplodeSound;

	//target to track
	UPROPERTY(EditAnywhere)
		AActor* target;

	UPROPERTY(EditAnywhere)
		float Speed;

	//minimum distance between it and target
	UPROPERTY(EditAnywhere)
		float MinimumDistance;

	UPROPERTY(EditAnywhere)
		float bombDelay;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float startSec;
	float currentSec;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void explodeNow();

};
