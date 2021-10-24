// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


//niagara specific header
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraSystemInstance.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"

class AAwooCharacter;

UCLASS()
class AWOO_API ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

	UPROPERTY(EditAnywhere)
		float RotationSpeed;

	//laser length
	UPROPERTY(EditAnywhere)
		float Distance;

	//damage rate
	UPROPERTY(EditAnywhere)
		float DamageRate;

	//laser color
	UPROPERTY(EditAnywhere)
		FLinearColor LaserColor;

	//niagara system
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* LaserFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* LaserImpactFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraComponent* NLaser;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraComponent* NLaserImpact;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector TraceEnd;

	FVector HitLoc;

	bool HitStuff;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//trace functions declare
	bool Trace(
		UWorld* World,
		TArray<AActor*>& ActorsToIgnore,
		const FVector& Start,
		const FVector& End,
		FHitResult& HitOut,
		ECollisionChannel CollisionChannel,
		bool ReturnPhysMat
	);

	UFUNCTION(BlueprintCallable, Category = "Tracing")
		void CallMyTrace();

	void ProcessTraceHit(FHitResult& HitOut);
};
