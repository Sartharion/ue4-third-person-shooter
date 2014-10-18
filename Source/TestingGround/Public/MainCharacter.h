

#pragma once

#include "IDamageable.h"
#include "BallProjectile.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AMainCharacter : public ACharacter, public IDamageable
{
	GENERATED_UCLASS_BODY()

	/** Camera boom, positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TSubobjectPtr<USpringArmComponent> CameraBoom;

	/** Extension for the CameraBoom. Used for OTS(Over The Shoulder) view */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TSubobjectPtr<USpringArmComponent> CameraBoomExtension;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TSubobjectPtr<UCameraComponent> FollowCamera;

	/** The speed of the character while jogging */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float JogSpeed;

	/** The speed of the character while sprinting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float SprintSpeed;

	/** The speed of the character while aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float AimSpeed;

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
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	bool bIsReloading;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector GunOffset;

	/** The length of the camera boom while the character is aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float CameraBoomLengthWhileAiming;

	/** The length of the camera boom extension while the character is aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float CameraBoomExtensionLengthWhileAiming;

	/** The smooth speed at which the camera transitions between 2 points in space (A multipllier for DeltaTime) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float CameraTransitionSmoothSpeed;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ABallProjectile> ProjectileClass;

	/** The current health that the character has left */
	UPROPERTY(BlueprintReadOnly, Category = "Character | Gameplay")
	float Health;

	/** The maximum health of the character that he can have left */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character | Gameplay")
	float HealthCapacity;

	/** How many projectiles the character can fire each second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay | Projectile")
	int32 ShotsPerSecond;

	/** The ammo capacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay | Projectile")
	int32 AmmoCapacity;

	/** The clip capacity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay | Projectile")
	int32 ClipCapacity;

	/** The current amount of ammo that the character has */
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay | Projectile")
	int32 Ammo;

	/** The amount of ammo loaded in the clip */
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay | Projectile")
	int32 AmmoInClip;

	/** Reloads the clip of the character */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	void Reload();

	/**
	 * The character picks up a specified amount of ammo
	 * Returns the amount of ammo that the character picked up (He may not pick all of the ammo if he reaches the AmmoCapacity)
	 * @param AmmoAmount - The amount of ammo that the character picks up
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	int32 PickUpAmmo(int32 AmmoAmount);

	/**
	 * The character takes damage.
	 * @param Damage - The amount of damage that the character gets
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	virtual void TakeDamage(float Damage) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
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

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn unterface

private:
	/** The delay between the firing of projectiles in seconds */
	float FireDelay;

	/** This is the counter for the FireDelay. The counter counts the seconds and when it reaches the FireDelay the OnFire() is called */
	float FireDelayCounter;

	/** The starting length of the camera boom */
	float CameraBoomLengthCache;

	/** The starting length of the camera boom extension */
	float CameraBoomExtensionLengthCache;

	/** Moves the camera closer to the character */
	void MoveCameraCloserToCharacter(float TransitionSmoothSpeed, float DeltaTime);

	/** Moves the camera away from the character */
	void MoveCameraAwayFromCharacter(float TransitionSmoothSpeed, float DeltaTime);
};
