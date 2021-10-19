// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "Item.h"
#include "EnemyCharacter.h"

//niagara specific header
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraSystemInstance.h"

#include "StunGrenade.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStunTestEventDispatcher);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStunEventDispatcher,FVector,grenadeLoc,float,stunTime,float,stunRadius);

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
		float offsetDistance;

	//niagara system
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UNiagaraSystem* ParticleFX;

	UNiagaraComponent* effect;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//// declare overlap begin function
	//UFUNCTION()
	//	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//event dispatcher
	UPROPERTY(BlueprintAssignable)
		FStunEventDispatcher StunEvent;

	UPROPERTY(BlueprintAssignable)
		FStunTestEventDispatcher MyTestEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//for tracking
	float startSecond;
	float currentSecond;

	bool stunStart;

	bool finishAim;

	//trigger component
	USphereComponent* TriggerSphere;
};