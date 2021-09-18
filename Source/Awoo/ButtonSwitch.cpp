// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonSwitch.h"
#include "AwooCharacter.h"

// Sets default values
AButtonSwitch::AButtonSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	TriggerVolume->SetupAttachment(BaseMesh);
}

// Called when the game starts or when spawned
void AButtonSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	//bind function
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AButtonSwitch::OnBoxOverlapBegin);
}

// Called every frame
void AButtonSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

void AButtonSwitch::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)//not with itself
	{
		//UE_LOG(LogTemp, Warning, TEXT("Overlap has begun"));
		//check if player overlap
		if (Cast<AAwooCharacter>(OtherActor))
		{
			ToggleLightEvent.Broadcast();
		}
		
	}
}