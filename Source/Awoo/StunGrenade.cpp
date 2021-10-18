// Fill out your copyright notice in the Description page of Project Settings.


#include "StunGrenade.h"
#include "AwooCharacter.h"
#include "DrawDebugHelpers.h"

AStunGrenade::AStunGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//default name and desc
	ItemName = FString(TEXT("Smoke Grenade"));
	ItemDesc = FString(TEXT("Release poisonous smoke to stun enemies"));

	//default values
	stunSec = 3;
	startSecond = 0;
	stunStart = false;
	finishAim = false;
}


// Called when the game starts or when spawned
void AStunGrenade::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStunGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//set location based on player when equipped
	if (gameChar)
	{
		if (gameChar->ItemEquipped == this)
		{
			SetActorLocation(gameChar->GetActorLocation() + offsetLoc);
		}
	}
	
	//update current second
	currentSecond = GetWorld()->UWorld::GetRealTimeSeconds();

	if (stunStart && currentSecond - startSecond > stunSec)
	{
		stunStart = false;

		Destroy();
	}

	//activated , stun enemies
	if (stunStart)
	{
		//broadcast stun event
	}

	if (finishAim)
	{
		//draw debug lines
		DrawDebugSphere(GetWorld(), GetActorLocation(), stunRadius, 26, FColor(181, 0, 0), false, 1, 0, 2);
	}

}


//specific function when item used by player
void AStunGrenade::UseItem()
{
	
	if (finishAim)
	{
	
		//spawn particle system
		ParticleFX = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/NiagaraFX/spriteEffect/smokeSystem.smokeSystem'"), nullptr, LOAD_None, nullptr);

		UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ParticleFX,
			GetActorLocation(), //position
			FRotator(1),
			FVector(0.05, 0.05, 0.05), //scale
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true);

		//start effect
		stunStart = true;
		startSecond = currentSecond;

		isUsed = true;
		gameChar->MessageString = FString(TEXT("Smoke grenade placed!Bots nearby stunned!"));
	}

	if (!isUsed)
	{
		//1st time for aiming
		gameChar->MessageString = FString(TEXT("move to set grenade location, press Q again to throw"));

		finishAim = true;
	}
}