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

	//TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	//TriggerVolume->SetupAttachment(BaseMesh);

	isMyEventBound = false;
}

// Called when the game starts or when spawned
void AButtonSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	////bind function
	//TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AButtonSwitch::OnBoxOverlapBegin);
}

// Called every frame
void AButtonSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

//overlap to trigger switch
//altered approach due to unreal detecting overlap twice
//void AButtonSwitch::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor && OtherActor != this)//not with itself
//	{
//		
//		//check if player overlap
//		if (Cast<AAwooCharacter>(OtherActor))
//		{
//			ToggleLightEvent.Broadcast();
//			UE_LOG(LogTemp, Warning, TEXT("Overlap has begun"));
//		}
//		
//	}
//}


void AButtonSwitch::ButtonPressed()
{
	ToggleLightEvent.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Button pressed"));
}


//show button status when raycasted
void AButtonSwitch::Interact_Implementation(AActor* target)
{

	gameChar = Cast<AAwooCharacter>(target);

	if (gameChar)
	{

		gameChar->MessageString = FString(TEXT("Hit F to press or release button."));

	}

	//bind toggle function to event once;
	if (!isMyEventBound)
	{
		gameChar->FlipSwitchEvent.AddDynamic(this, &AButtonSwitch::ButtonPressed);

		isMyEventBound = true;
	}
}
