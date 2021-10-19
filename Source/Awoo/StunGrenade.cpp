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
			SetActorLocation((gameChar->GetActorForwardVector()* offsetDistance) + gameChar->GetActorLocation());
		}
	}
	
	//update current second
	currentSecond = GetWorld()->UWorld::GetRealTimeSeconds();


	//check to end stun
	if (stunStart)
	{
	
		if (currentSecond - startSecond > stunSec)
		{
			stunStart = false;

			//stop niagara system
			effect->GetSystemInstance()->Deactivate(false);

			Destroy();
		}
	}

	//start aiming
	if (finishAim)
	{
		//draw debug lines
		DrawDebugSphere(GetWorld(), GetActorLocation(), stunRadius, 26, FColor(181, 0, 0), false, 0.05, 0, 2);
	}

}


//specific function when item used by player
void AStunGrenade::UseItem()
{
	//UE_LOG(LogTemp, Warning, TEXT("use grenade!!"));
	
	if (finishAim)
	{
	
		//spawn niagara system
		//ParticleFX = LoadObject<UNiagaraSystem>(nullptr, TEXT("NiagaraSystem'/Game/NiagaraFX/spriteEffect/smokeSystem.smokeSystem'"), nullptr, LOAD_None, nullptr);

		if (ParticleFX)
		{
			effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ParticleFX,
				GetActorLocation(), //position
				FRotator(1),
				FVector(1, 1, 1), //scale
				true,
				true,
				ENCPoolMethod::AutoRelease,
				true);

		}
		
		//start effect
		stunStart = true;
		startSecond = currentSecond;

	
		//broadcast stun event
		//pass stun parameters
		StunEvent.Broadcast(GetActorLocation(), stunSec, stunRadius);
	
		gameChar->MessageString = FString(TEXT("Smoke grenade placed!Bots nearby stunned!"));

	
		isUsed = true;

	}

	if (!isUsed)
	{
		//1st time for aiming
		gameChar->MessageString = FString(TEXT("move to set grenade location, press Q again to throw"));

		finishAim = true;

	}
}

