// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 5, 0.f);

	GetMesh()->SetCollisionProfileName(TEXT("Trigger"));
	
	//default values
	patrolIndex = 0;
	ChaseSpeedModifier = 0.8;
	DamageValue = 5;
	isStun = false;
	stunStartSec = 0;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//get player and its walk speed
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	float playerSpeed;
	if (player)
	{
		playerSpeed = player->GetCharacterMovement()->MaxWalkSpeed;

		//set ai to 10% slower than player
		GetCharacterMovement()->MaxWalkSpeed = (ChaseSpeedModifier * playerSpeed);

		UE_LOG(LogTemp, Warning, TEXT("player speed found!"));
	}

	//bind event
	TArray<AActor*>foundGrenade;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStunGrenade::StaticClass(), foundGrenade);

	for (AActor* actorRef : foundGrenade)
	{
		AStunGrenade* tempGrenade = Cast<AStunGrenade>(actorRef);

		if (tempGrenade)
		{
			tempGrenade->StunEvent.AddDynamic(this, &AEnemyCharacter::updateStun);
		}
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//update current second
	currentSec = GetWorld()->UWorld::GetRealTimeSeconds();

	if (isStun)
	{
		UE_LOG(LogTemp, Warning, TEXT("enemy now stunned!!"));
		if (currentSec - stunStartSec > myStunTime)
		{
			isStun = false;
		}
	}
	
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::updateStun(FVector grenadeLoc, float stunTime, float stunRadius)
{
	if (FVector::Dist(GetActorLocation(), grenadeLoc) < stunRadius)
	{
		isStun = true;

		myStunTime = stunTime;

		stunStartSec = currentSec;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("enemy too far from stun grenade!!"));
	}
}