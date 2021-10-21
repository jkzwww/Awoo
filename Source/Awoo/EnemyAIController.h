// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Touch.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class AWOO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AEnemyAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	//void OnMoveCompleted(FAIRequestID RequetID, const FPathFollowingResult& Result)override;

	void GenerateNewRandomLocation();

	void LaunchBomb();

	UFUNCTION()
		void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float SightRadius = 500.f;

	//how long Ai maintain last known position in memory
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float SightAge = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float LoseSightRadius = SightRadius + 30.f;

	//peripheral vision (degrees)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float FieldOfView = 45.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		UAISenseConfig_Sight* SightConfiguration;

	////how long Ai maintain last known position in memory
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	//	float TouchAge = 1.f;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	//	UAISenseConfig_Touch* TouchConfiguration;

	UNavigationSystemV1* NavigationSystem;
	APawn* TargetPlayer;


	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardData* AIBlackboard;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardComponent* BlackboardComponent;
};
