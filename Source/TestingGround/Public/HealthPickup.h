// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AHealthPickup : public APickup
{
	GENERATED_UCLASS_BODY()

	/** The amount of health that the pickup contains */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content")
	float Health;

	void OnPickedUp(APawn* Picker) override;	
};

