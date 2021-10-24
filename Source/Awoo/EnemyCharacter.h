// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Bomb.h"
#include "StunGrenade.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class APet;
class UAnimMontage;
class USkeletalMeshComponent;

UENUM(BlueprintType)
enum class EEnemyType : uint8 {
	ET_GUARD      UMETA(DisplayName = "guardian"),
	ET_SEEK       UMETA(DisplayName = "seeker"),
	ET_BOMBER	  UMETA(DisplayName = "bomber"),
};

UCLASS()
class AWOO_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//health
	float HP;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** AnimMontage to play when dead */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* DeadAnimation;

	//materials
	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Patrol")
		EEnemyType myPatrolType;

	//patrol points for guardian
	UPROPERTY(EditAnywhere, Category = "Patrol")
		TArray<FVector> PatrolLocs;

	//speed modifier
	UPROPERTY(EditAnywhere, Category = "Attack")
		float ChaseSpeedModifier;

	//damage value on player when touched
	UPROPERTY(EditAnywhere, Category = "Attack")
		float DamageValue;

	//bomb class to spawn
	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<class ABomb> ToSpawn;

	//which point currently in
	int patrolIndex;

	//whether stunned
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Attack")
		bool isStun;

	float myStunTime;

	float stunStartSec;
	float currentSec;

	UFUNCTION(BlueprintCallable)
		void updateStun(FVector grenadeLoc,float stunTime,float stunRadius);

	//whether charmed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool isCharmed;

	//pet location
	FVector petLoc;

	UFUNCTION(BlueprintCallable)
		void updateCharmStat(bool petCharm,FVector newLoc);

	//bomb attack
	void BombPlayer(AActor* targetPlayer);

	//take damage
	void TakeDamage(float damageVal);
};
