// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "Weapon.h"

FWeapon::FWeapon(int32 AmmoCapacity, int32 ClipCapacity, TSubclassOf<AProjectileBase> ProjectileClass)
{
	this->SetAmmoCapacity(AmmoCapacity);
	this->SetClipCapacity(ClipCapacity);
	this->SetRemainingAmmo(this->AmmoCapacity - this->ClipCapacity);
	this->SetAmmoInClip(this->ClipCapacity);
	this->SetProjectileClass(ProjectileClass);
}

FWeapon::~FWeapon()
{
}

int32 FWeapon::GetAmmoCapacity() const
{
	return this->AmmoCapacity;
}

void FWeapon::SetAmmoCapacity(int32 AmmoCapacity)
{
	this->AmmoCapacity = AmmoCapacity;
}

int32 FWeapon::GetClipCapacity() const
{
	return this->ClipCapacity;
}

void FWeapon::SetClipCapacity(int32 ClipCapacity)
{
	this->ClipCapacity = ClipCapacity;
}

int32 FWeapon::GetRemainingAmmo() const
{
	return this->RemainingAmmo;
}

void FWeapon::SetRemainingAmmo(int32 RemainingAmmo)
{
	this->RemainingAmmo = RemainingAmmo;
	if (this->RemainingAmmo > this->AmmoCapacity)
	{
		this->RemainingAmmo = this->AmmoCapacity;
	}
}

int32 FWeapon::GetAmmoInClip() const
{
	return this->AmmoInClip;
}

void FWeapon::SetAmmoInClip(int32 AmmoInClip)
{
	this->AmmoInClip = AmmoInClip;
	if (this->AmmoInClip > ClipCapacity)
	{
		this->AmmoInClip = ClipCapacity;
	}
}

TSubclassOf<AProjectileBase> FWeapon::GetProjectileClass() const
{
	return this->ProjectileClass;
}

void FWeapon::SetProjectileClass(TSubclassOf<AProjectileBase> ProjectileClass)
{
	this->ProjectileClass = ProjectileClass;
}

void FWeapon::Reload()
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

