// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "Weapon.h"


UWeapon::UWeapon(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void UWeapon::Reload()
{
	if ((this->AmmoInClip < this->ClipCapacity) && (this->RemainingAmmo > 0))
	{
		int32 AmmoToReload = this->ClipCapacity - this->AmmoInClip;

		if (this->RemainingAmmo > AmmoToReload)
		{
			this->AmmoInClip += AmmoToReload;
			this->RemainingAmmo -= AmmoToReload;
		}
		else
		{
			this->AmmoInClip += this->RemainingAmmo;
			this->RemainingAmmo = 0;
		}
	}
}

int32 UWeapon::AddAmmo(int32 Ammo)
{
	int32 AddedAmount = Ammo;

	this->RemainingAmmo += Ammo;
	if (this->RemainingAmmo > this->AmmoCapacity)
	{
		AddedAmount = Ammo - (this->RemainingAmmo - this->AmmoCapacity);
		this->RemainingAmmo = this->AmmoCapacity;
	}

	return AddedAmount;
}
