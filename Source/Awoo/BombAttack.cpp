// Fill out your copyright notice in the Description page of Project Settings.


#include "BombAttack.h"

EBTNodeResult::Type UBombAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;

	if (!Component)
		return EBTNodeResult::Failed;

	AEnemyAIController* MyController = Cast<AEnemyAIController>(Component->GetOwner());

	if (!MyController)
		return EBTNodeResult::Failed;

	MyController->LaunchBomb();

	return	EBTNodeResult::Succeeded;
}