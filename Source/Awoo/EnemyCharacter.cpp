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
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

