// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPawnDetected);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnSensesUpdated);

	//touch sense
	//TouchConfiguration = CreateDefaultSubobject<UAISenseConfig_Touch>(TEXT("Touch Configuration"));
	//TouchConfiguration->SetMaxAge(TouchAge);
	//GetPerceptionComponent()->ConfigureSense(*TouchConfiguration);

	TargetPlayer = nullptr;


}


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!AIBlackboard)
		return;

	if (!ensure(BehaviorTree))
		return;

	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);

	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	GenerateNewRandomLocation();

}


void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}


void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());	
		//UE_LOG(LogTemp, Warning, TEXT("player position updated!!"));

	}
	
	AEnemyCharacter* myEnemyChar = Cast<AEnemyCharacter>(GetPawn());

	if (myEnemyChar)
	{

		//UE_LOG(LogTemp, Warning, TEXT("pawn found!!"));
		if (myEnemyChar->isStun)
		{
			//UE_LOG(LogTemp, Warning, TEXT("enemy now stunned!!"));
			BlackboardComponent->SetValueAsBool("getStun", true);
		}
		else
		{
			BlackboardComponent->ClearValue("getStun");
		}


		if (myEnemyChar->isCharmed)
		{
			UE_LOG(LogTemp, Warning, TEXT("enemy now charmed!!"));
			BlackboardComponent->SetValueAsBool("getCharmed", true);
			BlackboardComponent->SetValueAsVector("PetPosition", myEnemyChar->petLoc);
		}
		else
		{
			BlackboardComponent->ClearValue("getCharmed");
		}
		
		if (myEnemyChar->myPatrolType == EEnemyType::ET_BOMBER)
		{
			if (TargetPlayer)
			{
				if (FVector::Dist(myEnemyChar->GetActorLocation(), TargetPlayer->GetActorLocation()) > myEnemyChar->LostDistance)
				{
					BlackboardComponent->ClearValue("canBomb");
					TargetPlayer = nullptr;
				}
			}
		}
		
	}

	
}


FRotator AEnemyAIController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}

	return FRotator(0, 0, 0);
}


void AEnemyAIController::GenerateNewRandomLocation()
{
	if (NavigationSystem)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Nav System Found"));
		FNavLocation returnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, returnLocation);

		AEnemyCharacter* myEnemy = Cast<AEnemyCharacter>(GetPawn());

		if (myEnemy)
		{
			//if it is a guardian, set location manually
			if (myEnemy->myPatrolType == EEnemyType::ET_GUARD)
			{
				myEnemy->patrolIndex++;

				if (myEnemy->patrolIndex == myEnemy->PatrolLocs.Num())
				{
					myEnemy->patrolIndex = 0;
				}

				returnLocation.Location = myEnemy->PatrolLocs[myEnemy->patrolIndex];

			}


		}

		/*MoveToLocation(returnLocation.Location);*/
		BlackboardComponent->SetValueAsVector("PatrolPoint", returnLocation.Location);
	}
}


void AEnemyAIController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{

	//FAISenseID sightid = SightConfiguration->GetSenseID();
	//FAISenseID touchid = TouchConfiguration->GetSenseID();

	UE_LOG(LogTemp, Warning, TEXT("Sensed"));

	APawn* TemporaryPawn = Cast<APawn>(UpdatedActor);

	AEnemyCharacter* myEnemyChar = Cast<AEnemyCharacter>(GetPawn());


	if (TemporaryPawn && TemporaryPawn->IsPlayerControlled())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Set Actor Location"));
			TargetPlayer = TemporaryPawn;
			
			if (myEnemyChar->myPatrolType == EEnemyType::ET_BOMBER)
			{
				BlackboardComponent->SetValueAsBool("canBomb", true);
			}
			else
			{
				BlackboardComponent->SetValueAsBool("ChasePlayer", true);
				BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());

			}
		
			/*if (Stimulus.Type == sightid)
			{
				UE_LOG(LogTemp, Warning, TEXT("target seen!!"));
			}*/

			/*if (Stimulus.Type == touchid)
			{
				UE_LOG(LogTemp, Warning, TEXT("touched target!"));
			}*/

		}
	}
	else
	{
		TargetPlayer = nullptr;
		BlackboardComponent->ClearValue("ChasePlayer");
		BlackboardComponent->ClearValue("canBomb");
	}

	//Stimulus.ReceiverLocation == 

}


//attack task
void AEnemyAIController::LaunchBomb()
{
	if (TargetPlayer)
	{
		AEnemyCharacter* myEnemyChar = Cast<AEnemyCharacter>(GetPawn());

		if (myEnemyChar)
		{
			myEnemyChar->BombPlayer(TargetPlayer);
		}

	}

}
