// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "AwooCharacter.h"

// Sets default values
ABomb::ABomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(RootComponent);

	//default
	Speed = 200;
	MinimumDistance = 80;

}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();

	//update current second
	if (GetWorld())
	{
		startSec = GetWorld()->UWorld::GetRealTimeSeconds();
	}


}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentSec = GetWorld()->UWorld::GetRealTimeSeconds();

	if (target)
	{
		//moving direction
		FVector TargetDirection = target->GetActorLocation() - GetActorLocation();

		if (TargetDirection.Size() > MinimumDistance)
		{
			TargetDirection.Normalize();

			SetActorLocation(GetActorLocation() + TargetDirection * Speed * DeltaTime);
		}
	}

	if (currentSec - startSec > bombDelay)
	{
		explodeNow();
	}

}


void ABomb::explodeNow()
{
	if (ExplodeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation(), 1.0F, 1.0F, 0.0F, nullptr, nullptr);
	}

	if (BombParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BombParticle, GetActorLocation(), FRotator::ZeroRotator, true);
	}

	if (target)
	{
		FVector TargetDirection = target->GetActorLocation() - GetActorLocation();

		if (TargetDirection.Size() < damageRad)
		{
			AAwooCharacter* myChar = Cast<AAwooCharacter>(target);

			if (myChar)
			{
				myChar->ReceiveDamage(damageVal);
			}
		}
	}

	Destroy();
}