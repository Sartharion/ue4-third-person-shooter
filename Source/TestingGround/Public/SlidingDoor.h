

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

	/** The static mesh of the door frame */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sliding Door")
	TSubobjectPtr<UStaticMeshComponent> DoorFrame;

	/** The static mesh of the left wing of the door */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sliding Door")
	TSubobjectPtr<UStaticMeshComponent> LeftWing;

	/** The static mesh of the right wing of the door */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sliding Door")
	TSubobjectPtr<UStaticMeshComponent> RightWing;

	/** The root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TSubobjectPtr<UBoxComponent> TriggerVolume;

	/** A Delay before closing the door (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	float DelayBeforeClosing;

	/** The speed at which the door slides. A multiplier for the DeltaTime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	float SlideSpeed;

	/** Should the door use the slide vectors to determine the End locations of the door wings */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	bool bUseSlideVectors;

	/** Indicates if the door is active. The door slides only if active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	bool bIsActive;

	/** The slide vector from the start to the end location of the left wing of the door */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	FVector LeftWingSlideVector;

	/** The slide vectir from the start to the end location of the right wing of the door */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	FVector RightWingSlideVector;

	/** The start location of the left wing of the door (in world space) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	FVector LeftWingStartLocation;

	/** The end location of the left wing of the door (in world space) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	FVector LeftWingEndLocation;

	/** The start location of the right wing of the door (in world space) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	FVector RightWingStartLocation;

	/** The end location of the right wing of the door (in world space) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sliding Door")
	FVector RightWingEndLocation;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/** The counter that determines if the delay before closing has expired and the door need to be closed */
	float DelayBeforeClosingCounter;

	/** Indicates if the door must open */
	bool bDoorMustOpen;

	/** Indicates if the door must close */
	bool bDoorMustClose;

	/** An event called when the DoorTrigger begins overlap */
	UFUNCTION()
	virtual void OnDoorTriggerBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** An event called when the DoorTrigger ends overlap */
	UFUNCTION()
	virtual void OnDoorTriggerEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

