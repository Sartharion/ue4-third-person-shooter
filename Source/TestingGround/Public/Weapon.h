// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ProjectileBase.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
namespace EWeaponType
{
	enum Type
	{
		None,
		Pistol,
		Rifle,
		Sniper,
		Shotgun,
		GranadeLauncher
	};
}

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TESTINGGROUND_API UWeapon : public UObject
{
	GENERATED_UCLASS_BODY()

	/** The type of the weapon (Rifle, Pistol, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TEnumAsByte<EWeaponType::Type> WeaponType;

	/** The ammo capacity of the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoCapacity;

	/** The clip capacity of the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 ClipCapacity;

	/** The remaining ammo of the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 RemainingAmmo;

	/** The amount of ammo left in the clip of the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoInClip;

	/** Projectile class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AProjectileBase> ProjectileClass;

	/** Reloads the weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	/** Adds ammo to the weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 AddAmmo(int32 Ammo);
};
