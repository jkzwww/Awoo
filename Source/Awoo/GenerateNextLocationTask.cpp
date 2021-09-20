// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateNextLocationTask.h"

EBTNodeResult::Type UGenerateNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;

	if (!Component)
		return EBTNodeResult::Failed;

	AEnemyAIController* MyController = Cast<AEnemyAIController>(Component->GetOwner());

	if (!MyController)
		return EBTNodeResult::Failed;

	MyController->GenerateNewRandomLocation();

	return	EBTNodeResult::Succeeded;
}