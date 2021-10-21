// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "AwooCharacter.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(RootComponent);

	//create trigger capsule 
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Drone Capusle"));
	TriggerCapsule->InitCapsuleSize(55.0f, 96.0f);
	//same collision profile 
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	//bind event
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ADrone::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ADrone::OnOverlapEnd);

	//default values
	TravelDistance = 300;
	Speed = 200;
	Tolerance = 10;
	Direction = 1;
	ZigZagChoice = 0;
	Range = 700;
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	

	InitialPosition = GetActorLocation();
	StartingPosition = GetActorLocation();

	//TriggerCapsule->SetCapsuleSize(capsuleRad,capsuleHeight,true);
	//DrawDebugCapsule(GetWorld(),GetActorLocation(),capsuleHeight,capsuleRad,FRotator::Quaternion(FRotator(0,0,0)),FColor::Blue,true, -1, 0, 2);
	
	
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector forwardRight = GetActorForwardVector() + GetActorRightVector();
	FVector forwardLeft = GetActorForwardVector() - GetActorRightVector();

	FVector ZigOrZag[] = { forwardRight,forwardLeft };

	FVector CurrentPosition = GetActorLocation();

	FVector DirectionVector;
	FVector TargetPosition;

	DirectionVector = ZigOrZag[ZigZagChoice] * Direction;
	TargetPosition = StartingPosition + (DirectionVector * TravelDistance);
	CurrentPosition += DirectionVector * Speed * DeltaTime;

	if (FVector::Dist(CurrentPosition, TargetPosition) <= Tolerance)
	{
		CurrentPosition = TargetPosition;
		StartingPosition = CurrentPosition;

		//out of range , zig zag back in opp direction
		if (FVector::Dist(CurrentPosition, InitialPosition) > Range)
		{
			Direction *= -1;
		}
		else if (ZigZagChoice == 1) //change between 0 and 1 for zig zag movement
		{
			ZigZagChoice = 0;
		}
		else
		{
			ZigZagChoice++;
		}

	}

	SetActorLocation(CurrentPosition);
}


void ADrone::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AAwooCharacter* myChar = Cast<AAwooCharacter>(OtherActor);

		if (myChar)
		{
			//emits particle
			if (SparkParticle)
			{
				FiredSpark = UGameplayStatics::SpawnEmitterAttached(SparkParticle, VisibleComponent);
			}
			//damage
			myChar->ReceiveDamage(damageRate);
		}

	}
}


void ADrone::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AAwooCharacter* myChar = Cast<AAwooCharacter>(OtherActor);

		if (myChar)
		{
			//deactivate particle
			if (SparkParticle)
			{
				FiredSpark->DeactivateSystem();
			}
			
		}

	}
}