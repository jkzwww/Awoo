// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Drone.generated.h"

class AAwooCharacter;

UCLASS()
class AWOO_API ADrone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrone();

	UPROPERTY(EditAnywhere,Category = "roam")
		float TravelDistance;

	UPROPERTY(EditAnywhere, Category = "roam")
		float Speed;

	UPROPERTY(EditAnywhere, Category = "roam")
		float Direction;

	UPROPERTY(EditAnywhere, Category = "roam")
		float Tolerance;

	UPROPERTY(EditAnywhere, Category = "roam")
		float Range;

	//create visible component
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

	//create trigger capsule
	UPROPERTY(EditAnywhere, Category = "Attack")
		class UCapsuleComponent* TriggerCapsule;

	//distance to floor
	UPROPERTY(EditAnywhere, Category = "Attack")
		float capsuleHeight;

	//capsule radius
	UPROPERTY(EditAnywhere, Category = "Attack")
		float capsuleRad;

	//damage
	UPROPERTY(EditAnywhere, Category = "Attack")
		float damageRate;

	//particle system
	UPROPERTY(EditAnywhere, Category = "Attack")
		UParticleSystem* SparkParticle;

	UParticleSystemComponent* FiredSpark; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FVector StartingPosition;
	FVector InitialPosition;

	//index to choose between which diagonal part of zigzag
	int ZigZagChoice;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
