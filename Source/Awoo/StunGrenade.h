// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "Item.h"

//niagara specific header
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "StunGrenade.generated.h"

/**
 * 
 */
UCLASS()
class AWOO_API AStunGrenade : public AItem
{
	GENERATED_BODY()
	
public:
	AStunGrenade();

	virtual void UseItem() override;


	//stun seconds
	UPROPERTY(EditAnywhere)
		float stunSec;

	//stun radius
	UPROPERTY(EditAnywhere)
		float stunRadius;

	//location offset from player
	UPROPERTY(EditAnywhere)
		FVector offsetLoc;

	//smoke particle system
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* ParticleFX;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//for tracking
	float startSecond;
	float currentSecond;

	bool stunStart;

	bool finishAim;

	
};
