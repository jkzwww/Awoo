// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8 {
	ET_GUARD      UMETA(DisplayName = "guardian"),
	ET_SEEK       UMETA(DisplayName = "seeker"),
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	//which point currently in
	int patrolIndex;

	//whether stunned
	bool isStun;
	float myStunTime;

	float stunStartSec;
	float currentSec;

	UFUNCTION(BlueprintCallable)
		void updateStun(FVector grenadeLoc, float stunRadius, float stunTime);
};
