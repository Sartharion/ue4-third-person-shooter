

#pragma once

#include "Pickup.h"
#include "AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AAmmoPickup : public APickup
{
	GENERATED_UCLASS_BODY()

	/** The amount of ammo contained in the pickup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	int32 Ammo;

	void OnPickedUp(APawn* Picker) override;
};

