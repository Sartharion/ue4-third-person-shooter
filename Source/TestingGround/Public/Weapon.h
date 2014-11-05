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
UCLASS(Blueprintable)
class TESTINGGROUND_API UWeapon : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TEnumAsByte<EWeaponType::Type> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 ClipCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 RemainingAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoInClip;

	/** Projectile class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AProjectileBase> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 AddAmmo(int32 Ammo);
};
