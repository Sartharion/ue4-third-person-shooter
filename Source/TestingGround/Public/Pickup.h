

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API APickup : public AActor
{
	GENERATED_UCLASS_BODY()

	/** The collision component of the Pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* BoxComponent;

	/** The static mesh of the Pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	UStaticMeshComponent* PickupMesh;

	/** The sound played when the pickup is picked up */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Sound")
	USoundBase* PickupSound;

	/** If true, the Pickup can be picked up. If false, the Pickup can't be picked up */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	bool bIsActive;

	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) override;

protected:
	/** Called when the Pickup is picked up */
	virtual void OnPickedUp(APawn* Picker);
};
