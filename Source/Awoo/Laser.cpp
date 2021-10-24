// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h"
#include "AwooCharacter.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	//VisibleComponent->SetupAttachment(RootComponent);
	RootComponent = VisibleComponent;

	VisibleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	NLaser = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara laser component"));
	NLaser->SetupAttachment(VisibleComponent);

	NLaserImpact = CreateDefaultSubobject<UNiagaraComponent>(TEXT("laser impact component"));
	NLaserImpact->SetupAttachment(VisibleComponent);

	//default
	RotationSpeed = 20;
	DamageRate = 0.05;
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
	
	if (LaserFX)
	{
		//NLaser = UNiagaraFunctionLibrary::SpawnSystemAttached(LaserFX, VisibleComponent, FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::AutoRelease);
		NLaser->SetAsset(LaserFX);
		NLaser->SetColorParameter("Color", LaserColor);
		/*NLaser->SetVariableLinearColor("Color", LaserColor);
		NLaser->SetVariableVec3("LaserEnd", GetActorLocation() + (GetActorForwardVector() * Distance));*/
		NLaser->SetVectorParameter("LaserEnd", GetActorLocation() + (GetActorForwardVector() * Distance));
		NLaser->ActivateSystem();
	}

	if (LaserImpactFX)
	{
		//NLaserImpact = UNiagaraFunctionLibrary::SpawnSystemAttached(LaserImpactFX, VisibleComponent, FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepRelativeOffset, true, true, ENCPoolMethod::AutoRelease);
		NLaserImpact->SetAsset(LaserImpactFX);
		NLaserImpact->SetVisibility(false);
		NLaserImpact->SetColorParameter("Color", LaserColor);
	}

}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//rotator(pitch,yaw,roll)
	FRotator Rotator(0, RotationSpeed * DeltaTime, 0);
	FQuat RotationQuaternion(Rotator);

	AddActorLocalRotation(RotationQuaternion, false, nullptr, ETeleportType::None);

	CallMyTrace();

}


//perform trace called by callmytrace which sets up parameters
bool ALaser::Trace(
	UWorld* World,
	TArray<AActor*>& ActorsToIgnore,
	const FVector& Start,
	const FVector& End,
	FHitResult& HitOut,
	ECollisionChannel CollisionChannel = ECC_Visibility,
	bool ReturnPhysMat = false
) {

	// The World parameter refers to our game world (map/level) 
	// If there is no World, abort
	if (!World)
	{
		return false;
	}

	// Set up our TraceParams object
	FCollisionQueryParams TraceParams(FName(TEXT("My Trace")), true, ActorsToIgnore[0]);

	// Should we simple or complex collision?
	TraceParams.bTraceComplex = true;

	// We don't need Physics materials 
	TraceParams.bReturnPhysicalMaterial = ReturnPhysMat;

	// Add our ActorsToIgnore
	TraceParams.AddIgnoredActors(ActorsToIgnore);

	// When we're debugging it is really useful to see where our trace is in the world
	// We can use World->DebugDrawTraceTag to tell Unreal to draw debug lines for our trace
	// (remove these lines to remove the debug - or better create a debug switch!)
	const FName TraceTag("MyTraceTag");
	//World->DebugDrawTraceTag = TraceTag;
	TraceParams.TraceTag = TraceTag;


	// Force clear the HitData which contains our results
	HitOut = FHitResult(ForceInit);

	// Perform our trace
	World->LineTraceSingleByChannel
	(
		HitOut,		//result
		Start,	//start
		End, //end
		CollisionChannel, //collision channel
		TraceParams
	);

	// If we hit an actor, return true
	return (HitOut.GetActor() != NULL);
}


//calls trace
void ALaser::CallMyTrace()
{
	// Get the location of the camera (where we are looking from) and the direction we are looking in
	//const 
	FVector Start = GetActorLocation();
	//const 
	FVector ForwardVector = GetActorForwardVector();

	// How for in front of our character do we want our trace to extend?
	// ForwardVector is a unit vector, so we multiply by the desired distance
	//const 
	FVector End = Start + ForwardVector * Distance;
	TraceEnd = End;

	// Force clear the HitData which contains our results
	FHitResult HitData(ForceInit);

	// What Actors do we want our trace to Ignore?
	TArray<AActor*> ActorsToIgnore;

	//Ignore the player character - so you don't hit yourself!
	ActorsToIgnore.Add(this);

	// Call our Trace() function with the paramaters we have set up
	// If it Hits anything
	if (Trace(GetWorld(), ActorsToIgnore, Start, End, HitData, ECC_Visibility, false))
	{
		// Process our HitData
		if (HitData.GetActor())
		{

			//UE_LOG(LogClass, Warning, TEXT("This a testing statement. %s"), *HitData.GetActor()->GetName());
			ProcessTraceHit(HitData);

		}
		else
		{
			// The trace did not return an Actor
			// An error has occurred
			// Record a message in the error log
		}
	}
	else
	{
		// We did not hit an Actor
		//ClearPickupInfo();
		//set laser end to original laser length
		NLaser->SetVectorParameter("LaserEnd", TraceEnd);
	}

}


//process trace hit when interactable item is detected
void ALaser::ProcessTraceHit(FHitResult& HitOut)
{
	//set laser to end at hit location
	NLaser->SetVectorParameter("LaserEnd", HitOut.Location);

	//set laser impact at hit location
	NLaserImpact->SetWorldLocation(HitOut.Location);
	NLaserImpact->SetVisibility(true);

	AAwooCharacter* target = Cast<AAwooCharacter>(HitOut.GetActor());

	if (target)
	{
		target->ReceiveDamage(DamageRate);
	}

}
