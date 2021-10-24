// Copyright Epic Games, Inc. All Rights Reserved.

#include "AwooCharacter.h"
#include "AwooProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "SceneManagement.h"
#include "SceneInterface.h"
DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


//////////////////////////////////////////////////////////////////////////
// AAwooCharacter

AAwooCharacter::AAwooCharacter()
{

	//create trigger capsule for pickups
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capusle"));
	TriggerCapsule->InitCapsuleSize(55.0f, 96.0f);
	//same collision profile with pickup
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	//bind event
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AAwooCharacter::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AAwooCharacter::OnOverlapEnd);

	//default stats value
	maxHealth = 100;
	maxHunger = 100;
	maxHydration = 100;
	InvSlotsNum = 15;
	isProtected = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	//post process component
	myDamagePP = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Character Post Process"));
	myDamagePP->SetupAttachment(FirstPersonCameraComponent);
	myDamagePP->Settings = PPsettings;

	FirstPersonCameraComponent->PostProcessSettings = PPsettings;

	PPsettings.bOverride_ColorGamma = true;
	PPsettings.bOverride_VignetteIntensity = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

}

void AAwooCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	//reset stats
	health = maxHealth;
	hunger = maxHunger;
	hydration = maxHydration;

	//default color
	myBloodColor = FLinearColor::Red;
	effectSpeed = 5;
	DesiredGamma = FVector4(1, 1, 1, 1);
	myCurrentGamma = DesiredGamma;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAwooCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AAwooCharacter::OnFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AAwooCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AAwooCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAwooCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAwooCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAwooCharacter::LookUpAtRate);


	//Bind pause 
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AAwooCharacter::pauseGame);

	//bind collect
	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, &AAwooCharacter::Collect);

	//bind use
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AAwooCharacter::UseEquipItem);

	//bind drop
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AAwooCharacter::DropItem);

	//bind equip
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AAwooCharacter::Equip);

	//bind consume
	PlayerInputComponent->BindAction("Consume", IE_Pressed, this, &AAwooCharacter::Consume);

	//bind flip
	PlayerInputComponent->BindAction("Flip", IE_Pressed, this, &AAwooCharacter::FlipASwitch);

	//bind pet skills
	PlayerInputComponent->BindAction("Charm", IE_Pressed, this, &AAwooCharacter::ReleaseSkill<ESkillType::CHARM>);
	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &AAwooCharacter::ReleaseSkill<ESkillType::HEAL>);
	PlayerInputComponent->BindAction("Shield", IE_Pressed, this, &AAwooCharacter::ReleaseSkill<ESkillType::SHIELD>);

}

void AAwooCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AAwooProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AAwooProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AAwooCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAwooCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AAwooCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AAwooCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AAwooCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AAwooCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AAwooCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAwooCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AAwooCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AAwooCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AAwooCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AAwooCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}


//overlap functions
void AAwooCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));*/

		//implement interface if exist
	/*	if (OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
		
				IInteractable::Execute_Interact(OtherActor, this);

		}*/

		//if overlap with end point platform
		if (Cast<AEndPlatform>(OtherActor))
		{
			//player win
			GameOverEvent.Broadcast(1);
		}

		//if overlap with pit trap (mytrap)
		if (Cast<AMyTrap>(OtherActor))
		{
			//player die
			GameOverEvent.Broadcast(0);
		}

		//check overlap with enemy
		AEnemyCharacter* myEnemy = Cast<AEnemyCharacter>(OtherActor);

		if (myEnemy && !isProtected)
		{
			//player attacked
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Touched by enemy!!!"));
			
			isContacted = true;
			myDamageRate = myEnemy->DamageValue;
		}

	}
}

void AAwooCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
			//check overlap with enemy
		AEnemyCharacter* myEnemy = Cast<AEnemyCharacter>(OtherActor);

		if (myEnemy)
		{
			//player attacked
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Touched by enemy!!!"));

			isContacted = false;
		}
	}


}


//perform trace called by callmytrace which sets up parameters
bool AAwooCharacter::Trace(
	UWorld* World,
	TArray<AActor*>& ActorsToIgnore,
	const FVector& Start,
	const FVector& End,
	FHitResult& HitOut,
	ECollisionChannel CollisionChannel = ECC_Pawn,
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
	/*World->DebugDrawTraceTag = TraceTag;*/
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
void AAwooCharacter::CallMyTrace()
{
	// Get the location of the camera (where we are looking from) and the direction we are looking in
	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();

	// How for in front of our character do we want our trace to extend?
	// ForwardVector is a unit vector, so we multiply by the desired distance
	const FVector End = Start + ForwardVector * 1256;

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

	}

}


//process trace hit when interactable item is detected
void AAwooCharacter::ProcessTraceHit(FHitResult& HitOut)
{
	if (HitOut.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		//interaction occurs on key pressed
		//IInteractable::Execute_Interact(HitOut.GetActor(), this);

		AItem* myItem = Cast<AItem>(HitOut.GetActor());

		//show item info if item is detected
		if (myItem)
		{
			myItem->gameChar = this;
			InteractItem = myItem;
			ShowInfoEvent.Broadcast(myItem->ItemName, myItem->ItemDesc, myItem->ItemHowTo);
		}
		else
		{
			IInteractable::Execute_Interact(HitOut.GetActor(), this);
			DisplayMessageEvent.Broadcast(MessageString); 
		}

	}
	else
	{
		//UE_LOG(LogClass, Warning, TEXT("Actor is NOT Interactable!"));
		//ClearPickupInfo();
		InteractItem = nullptr;

	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TICK~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void AAwooCharacter::Tick(float DeltaTime) 
{ 
	Super::Tick(DeltaTime); 

	//drop hunger and hydro by constant rate
	//call gameover for zero health
	if (health <= 0)
	{
		GameOverEvent.Broadcast(0);
	}
	else
	{
		if (hunger > 0)
		{
			hunger -= hungerDrop;
		}

		if (hydration > 0)
		{
			hydration -= hydroDrop;
		}

		if (hunger <= 0 || hydration <= 0)
		{
			ReceiveDamage(healthDrop);
		}
	
		//contact damage
		if (isContacted)
		{
			ReceiveDamage(myDamageRate);
		}

	}

	myCurrentGamma = FMath::Lerp(myCurrentGamma,DesiredGamma,effectSpeed);

	PPsettings.ColorGamma = myCurrentGamma;

	if (myCurrentGamma.Equals(DesiredGamma,0.01))
	{
		DesiredGamma = FVector4(1, 1, 1, 1);
		effectSpeed = 0.3;
		PPsettings.VignetteIntensity = 0;
	}
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TICK~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//collect interacting item through key press into inventory
void AAwooCharacter::Collect()
{
	if (myInventory.Num() <= InvSlotsNum)
	{
		if (InteractItem)
		{
			//item event when collected
			//interact item pointer is changed regularly, take item address as pointer instead
			IInteractable::Execute_Interact(InteractItem, this);

			//add to inventory
			myInventory.Add(&(*InteractItem));

			DisplayMessageEvent.Broadcast(FString(TEXT("Item added to inventory[I]")));
		}


	}
	else
	{
		DisplayMessageEvent.Broadcast(FString(TEXT("Inventory FULL!! Drop some items")));
	}

}


//equip item directly when interacting through keypress
void AAwooCharacter::Equip()
{
	if (myInventory.Num() <= InvSlotsNum)
	{
		if (InteractItem)
		{
			//item event when collected
			IInteractable::Execute_Interact(InteractItem, this);

			//add to inventory
			myInventory.Add(&(*InteractItem));

			//equip directly
			ItemEquipped = &(*InteractItem);

			DisplayMessageEvent.Broadcast(FString(TEXT("Item equipped and added to inventory[I]")));

		}
	}
	else
	{
		DisplayMessageEvent.Broadcast(FString(TEXT("Inventory FULL!!Drop some items")));
	}

}


//use equipped item and remove item from inventory
void AAwooCharacter::UseEquipItem()
{
	//check item equipped
	if (ItemEquipped)
	{
		//call item UseItem function to broadcast event
		ItemEquipped->UseItem();

		if (ItemEquipped->isUsed)
		{
			//remove from inventory
			myInventory.RemoveSingle(ItemEquipped);

			//clear equip slot
			ItemEquipped = nullptr;
		}

		DisplayMessageEvent.Broadcast(MessageString);
	}


}


//drop equipped item nearby
void AAwooCharacter::DropItem()
{
	if (ItemEquipped)
	{
		//drop at somewhere offset to actor
		ItemEquipped->DropItem(GetActorLocation() + FVector(50,20,0));

		//remove from inventory
		myInventory.RemoveSingle(ItemEquipped);

		//clear equip slot
		ItemEquipped = nullptr;
		
	}

}


//use consumable items directly by key press when interacting
void AAwooCharacter::Consume()
{
	if (InteractItem)
	{
		if (InteractItem->isConsumable)
		{
			InteractItem->UseItem();

			DisplayMessageEvent.Broadcast(MessageString);

			IInteractable::Execute_Interact(InteractItem, this);
		}
	}

}


//pause game variable mutator
void AAwooCharacter::pauseGame()
{
	PausePressed = true;
}


//flip switch nearby
void AAwooCharacter::FlipASwitch()
{
	FlipSwitchEvent.Broadcast();
}


//choose skill
void AAwooCharacter::ReleaseSkill(ESkillType mySkill)
{
	switch (mySkill)
	{

	case ESkillType::CHARM:
	{
		PetSkillEvent.Broadcast(1);
		break;
	}

	case ESkillType::HEAL:
	{
		PetSkillEvent.Broadcast(2);
		break;
	}

	case ESkillType::SHIELD:
	{
		PetSkillEvent.Broadcast(3);
		break;
	}

	}

	DisplayMessageEvent.Broadcast(MessageString);
}


//get damage
void AAwooCharacter::ReceiveDamage(float damageRate)
{
	if (!isProtected)
	{
		health -= damageRate;

	}
	
	//start lerping to gamma
	effectSpeed = 0.8;
	DesiredGamma = FVector4(1,0,0,1);
	PPsettings.VignetteIntensity = 1.5;
}


