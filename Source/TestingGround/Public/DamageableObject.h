// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DamageableObject.generated.h"

/**
*
*/
UINTERFACE()
class TESTINGGROUND_API UDamageableObject : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class TESTINGGROUND_API IDamageableObject
{
	GENERATED_IINTERFACE_BODY()

public:
	/**
	* The IDamageableObject takes damage
	* @param Damage - the amount of damage
	* @param Hit - hit information
	* @param DamageCauser - the Actor that caused the damage (a projectile for example)
	*/
	virtual void TakeDamage(float Damage, const FHitResult& Hit, const AActor* DamageCauser) = 0;
};


