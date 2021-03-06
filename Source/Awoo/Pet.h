// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Interactable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pet.generated.h"


UENUM(BlueprintType)
enum class EPetState : uint8 {

	PET_FOLLOW      UMETA(DisplayName = "follow"),
	PET_ATTRACT      UMETA(DisplayName = "attract"),
	PET_HEAL	 UMETA(DisplayName = "healing"),
	PET_SHIELD	UMETA(DisplayName = "protect"),
};

UENUM(BlueprintType)
enum class EPetPhase : uint8 {

	PET_HUNGRY      UMETA(DisplayName = "hungry"),
	PET_HEALTHY      UMETA(DisplayName = "healthy"),
	PET_HAPPY	 UMETA(DisplayName = "happy"),
	PET_ENERGY  UMETA(DisplayName = "energitic"),
};

class AAwooCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharmEventDispatcher,bool,isCharm,FVector,myLoc);

UCLASS()
class AWOO_API APet : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APet();

	//mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* BaseMesh;

	//owner
	AAwooCharacter* myOwner;

	//skill state
	EPetState myState;

	//growth phase
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EPetPhase myPhase;

	//food
	int myFood;

	//cooldown
	UPROPERTY(EditAnywhere)
		float skillCD;

	//skill duration
	UPROPERTY(EditAnywhere)
		float skillTime;

	//skill ready
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
		bool skillReady;

	//follow speed
	UPROPERTY(EditAnywhere)
		float speed;

	//follow distance
	UPROPERTY(EditAnywhere)
		float followDist;

	//charm effect distance
	UPROPERTY(EditAnywhere)
		float charmDist;

	//heal rate
	UPROPERTY(EditAnywhere)
		float healRate;

	//materials
	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterialInterface* Material;

	UMaterialInstanceDynamic* matInstance;
	
	UPROPERTY(EditAnywhere,Category = "Appearance")
		FLinearColor phase1Color;

	UPROPERTY(EditAnywhere, Category = "Appearance")
		FLinearColor phase2Color;
	
	UPROPERTY(EditAnywhere, Category = "Appearance")
		FLinearColor phase3Color;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//vector to owner
	FVector toTarget;

	//track time
	float startSec;
	float currentSec;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//to show message when interacted
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AActor* target);

	virtual void Interact_Implementation(AActor* target) override;

	UFUNCTION()
		void getFood();

	UFUNCTION()
		void useSkill(int skill);

	UPROPERTY(BlueprintAssignable)
		FCharmEventDispatcher CharmEvent;

};
