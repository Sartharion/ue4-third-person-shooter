// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "RifleAmmoPickup.h"


ARifleAmmoPickup::ARifleAmmoPickup(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	this->PickUpAmmoMethodPtr = &AMainCharacter::PickUpRifleAmmo;
}


