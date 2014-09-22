

#pragma once

#include "GameFramework/Actor.h"
#include "SlidingDoor.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API ASlidingDoor : public AActor
{
	GENERATED_UCLASS_BODY()

	/** The root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TSubobjectPtr<UBoxComponent> TriggerVolume;

	/** The static mesh for the door */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TSubobjectPtr<UStaticMeshComponent> DoorMesh;

	/** The speed at which the door slides. A multiplier for the DeltaTime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide Properties")
	float SlideSpeed;

	/** The vector on which the door will slide */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide Properties")
	FVector SlideVector;

	/** A Delay before closing the door in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide Properties")
	float SlidingDoorCloseDelay;

	virtual void Tick(float DeltaTime) override;
	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;
	virtual void ReceiveActorEndOverlap(AActor* OtherActor) override;

private:
	FVector SlidingDoorStartLocation;
	FVector SlidingDoorEndLocation;
	bool bInitializedDoor;
	bool bDoorMustOpen;
	bool bDoorMustClose;
	float SlidingDoorCloseDelaySecondsCounter;
};

