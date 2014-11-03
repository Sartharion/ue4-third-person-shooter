#pragma once

class IDamageable
{
public:
	/**
	 * The IDamageable object takes damage
	 * @param Damage - the amount of damage
	 * @param Hit - hit information
	 * @param DamageCauser - the Actor that caused the damage (a projectile for example)
	 */
	virtual void TakeDamage(float Damage, const FHitResult& Hit, const AActor* DamageCauser) = 0;
};
