

#pragma once

#include "CharacterBase.h"
#include "IDamageable.h"
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
	USpringArmComponent* CameraBoom;

	/** Extension for the CameraBoom. Used for OTS(Over The Shoulder) view */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoomExtension;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	/** The length of the camera boom while the character is aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float CameraBoomLengthWhileAiming;

	/** The length of the camera boom extension while the character is aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float CameraBoomExtensionLengthWhileAiming;

	/** The smooth speed at which the camera transitions between 2 points in space (A multipllier for DeltaTime) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float CameraTransitionSmoothSpeed;

	/** The left and right mouse sensitivity*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MouseXSensitivity;

	/** The up and down mouse sensitivity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MouseYSensitivity;	

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	/**
	 * Turns the character left or right
	 * @param AxisValue - value in range [-1.0, 1.0], (-1 -> TurnLeft, 1 -> TurnRight)
	 */
	virtual void Turn(float AxisValue);

	/**
	 * The character looks up or down
	 * @param AxisValue - value in range [-1.0, 1.0], (-1 -> LookUp, 1 -> LookDown)
	 */
	virtual void LookUp(float AxisValus);

	/** Fires a projectile towards the crosshair  */
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
