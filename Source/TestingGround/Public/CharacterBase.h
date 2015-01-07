// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "DamageableObject.h"
#include "ProjectileBase.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

/**
 *
 */
UCLASS()
class TESTINGGROUND_API ACharacterBase : public ACharacter, public IDamageableObject
{
	GENERATED_UCLASS_BODY()	

	/** The speed of the character while jogging */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float JogSpeed;

	/** The speed of the character while sprinting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float SprintSpeed;

	/** The speed of the character while aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float AimSpeed;

	/** The current health that the character has left */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float Health;

	/** The maximum health of the character that he can have left */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character")
	float HealthCapacity;

	/** Indicates if the character is sprinting */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool bIsSprinting;

	/** Indicates if the character is aiming */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool bIsAiming;

	/** Indicates if the character is firing */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool bIsFiring;

	/** Indicates if the character is reloading */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool bIsReloading;

	/** Indicates if the character is dead */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool bIsDead;

	/** The class used for the rifle of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UWeapon> RifleClass;

	/** The rifle of the character */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UWeapon* Rifle;

	/** A reference to the current equipped weapon */
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	UWeapon* EquippedWeapon;

	/** The socket name of the gun's muzzle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName GunMuzzleSocketName;

	/** Gun muzzle's offset from the characters location. Use only if the character does not have weapon attached to him. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Gameplay")
	FVector GunMuzzleOffset;

	/** How many projectiles the character can fire each second, when he is using an autofire weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int32 ShotsPerSecond;

	/** Reloads the clip of the character */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	void Reload();

	/**
	 * The character picks up a specified amount of rifle ammo.
	 * @param AmmoAmount - The amount of ammo that the character picks up
	 * @return the amount of ammo that the character picked up (He may not pick all of the ammo if he reaches the AmmoCapacity)
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	int32 PickUpRifleAmmo(int32 AmmoAmount);

	/**
	 * The character gains a specified amount of health.
	 * @param HealthAmount - The amount of health that the character gains
	 * @return the amount of health that the character gained (He may not gain all of the health if he reaches the HealthCapacity)
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	float GainHealth(float HealthAmount);

	/**
	 * The character takes damage.
	 * @param Damage - The amount of damage that the character gets
	 * @param Hit - The resulting hit
	 * @param DamageCauser - The actor that caused the damage (a projectile for example)
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual void TakeDamage(float Damage, const FHitResult& Hit, const AActor* DamageCauser) override;

	/** Called internaly in the OnFire() function. Can be used for playing a fire animation */
	UFUNCTION(BlueprintImplementableEvent, Category = "Gun Action")
	void OnFireEvent();

	/** Called internaly in the ReloadStart() function. Can be used for playing reload animation */
	UFUNCTION(BlueprintImplementableEvent, Category = "Gun Action")
	void OnReloadEvent();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/** Used for moving forward and backward */
	virtual void MoveForward(float AxisValue);

	/** Used for moving right and left */
	virtual void MoveRight(float AxisValue);

	/** The character starts sprinting */
	virtual void SprintStart();

	/** The character stops sprinting */
	virtual void SprintStop();

	/** The character starts aiming */
	virtual void AimStart();

	/** The character stops aiming */
	virtual void AimStop();

	/** The character starts shooting */
	virtual void FireStart();

	/** The character stops shooting */
	virtual void FireStop();

	/** Fires a projectile */
	virtual void OnFire();

	/** The character starts reloading */
	virtual void ReloadStart();

	/** The character stops reloading */
	virtual void ReloadStop();

protected:
	/** A reference to the weapon skeletal mesh */
	USkeletalMeshComponent* GunMesh;

private:
	/** The delay between the firing of projectiles in seconds */
	float FireDelay;

	/** This is the counter for the FireDelay. The counter counts the seconds and when it reaches the FireDelay the OnFire() is called */
	float FireDelayCounter;
};

