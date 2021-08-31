// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/CapsuleComponent.h"
#include "Interactable.h"
#include "Item.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AwooCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~event dispatcher~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyEventDispatcher, bool, isWinning);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FInfoEventDispatcher, FString, name,FString,desc,FString,howTo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageDispatcher, FString, myMessage);

UCLASS(config=Game)
class AAwooCharacter : public ACharacter
{
	GENERATED_BODY()

	//create trigger capsule
	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* TriggerCapsule;


	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* L_MotionController;

public:
	AAwooCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AAwooProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~custom variables~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//gamestates
	//track pause game event
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gamestats)
		bool PausePressed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float maxHydration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float maxHunger;

	//character stats
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
		float health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
		float hunger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
		float hydration;

	//character stats droprate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharSetting")
		float healthDrop = 0.001;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharSetting")
		float hungerDrop = 0.04;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharSetting")
		float hydroDrop = 0.05;

	//message to display when pickup / win / lose
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Display")
		FString MessageString= FString(TEXT(""));

	//item interacting with
	AItem* InteractItem;

	//item equipped
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
		AItem* ItemEquipped = nullptr;
	
	//inventory for items
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Display")
		TArray <AItem*> myInventory;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~function declare~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//game state variables mutator
	void pauseGame();

	//kill player
	void killMe();

	//declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//trace functions declare
	bool Trace(
		UWorld* World,
		TArray<AActor*>& ActorsToIgnore,
		const FVector& Start,
		const FVector& End,
		FHitResult& HitOut,
		ECollisionChannel CollisionChannel,
		bool ReturnPhysMat
	);

	UFUNCTION(BlueprintCallable,Category = "Tracing")
		void CallMyTrace();

	void ProcessTraceHit(FHitResult& HitOut);

	
	//tick
	virtual void Tick(float DeltaTime) override;


	//item related functions
	void Collect();

	void Equip();

	void UseEquipItem();

	void DropItem();

	void Consume();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~event~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	UPROPERTY(BlueprintAssignable, Category = "GameState")
		FMyEventDispatcher GameOverEvent;

	UPROPERTY(BlueprintAssignable, Category = "Tracing")
		FInfoEventDispatcher ShowInfoEvent;

	UPROPERTY(BlueprintAssignable, Category = "Display")
		FMessageDispatcher DisplayMessageEvent;
};

