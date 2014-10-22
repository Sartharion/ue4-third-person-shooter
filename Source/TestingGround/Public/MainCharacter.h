

#pragma once

#include "CharacterBase.h"
#include "IDamageable.h"
#include "BallProjectile.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AMainCharacter : public ACharacterBase
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

	/** The length of the camera boom while the character is aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float CameraBoomLengthWhileAiming;

	/** The length of the camera boom extension while the character is aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float CameraBoomExtensionLengthWhileAiming;

	/** The smooth speed at which the camera transitions between 2 points in space (A multipllier for DeltaTime) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float CameraTransitionSmoothSpeed;

	/**
	 * The character picks up a specified amount of ammo
	 * Returns the amount of ammo that the character picked up (He may not pick all of the ammo if he reaches the AmmoCapacity)
	 * @param AmmoAmount - The amount of ammo that the character picks up
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	int32 PickUpAmmo(int32 AmmoAmount);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnFire() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn unterface

private:
	/** The starting length of the camera boom */
	float CameraBoomLengthCache;

	/** The starting length of the camera boom extension */
	float CameraBoomExtensionLengthCache;

	/** Moves the camera closer to the character */
	void MoveCameraCloserToCharacter(float TransitionSmoothSpeed, float DeltaTime);

	/** Moves the camera away from the character */
	void MoveCameraAwayFromCharacter(float TransitionSmoothSpeed, float DeltaTime);
};
