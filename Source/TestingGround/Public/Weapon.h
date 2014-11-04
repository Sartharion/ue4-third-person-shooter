// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectileBase.h"

/**
 * 
 */
class TESTINGGROUND_API FWeapon
{
public:
	FWeapon(int32 AmmoCapacity, int32 ClipCapacity, TSubclassOf<AProjectileBase> ProjectileClass);
	~FWeapon();

	int32 GetAmmoCapacity() const;
	void SetAmmoCapacity(int32 AmmoCapacity);

	int32 GetClipCapacity() const;
	void SetClipCapacity(int32 ClipCapacity);

	int32 GetRemainingAmmo() const;
	void SetRemainingAmmo(int32 RemainingAmmo);

	int32 GetAmmoInClip() const;
	void SetAmmoInClip(int32 AmmoInClip);

	TSubclassOf<AProjectileBase> GetProjectileClass() const;
	void SetProjectileClass(TSubclassOf<AProjectileBase> ProjectileClass);

	void Reload();

private:
	/** The total ammo capacity */
	int32 AmmoCapacity;

	/** The clip capacity */
	int32 ClipCapacity;

	/** The remaining ammo (the ammo in the clip in not included) */
	int32 RemainingAmmo;

	/** The ammo loaded in the clip of the weapon */
	int32 AmmoInClip;

	/** A projectile class */
	TSubclassOf<AProjectileBase> ProjectileClass;
};
