

#pragma once

#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AMainCharacter : public ACharacter
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

	virtual void BeginPlay() override;

protected:
	/** Used for moving forward and backward */
	void MoveForward(float AxisValue);

	/** Used for moving right and left */
	void MoveRight(float AxisValue);

	/** The character starts sprinting */
	void SprintStart();

	/** The character stops sprinting */
	void SprintStop();

	/** The character starts aiming */
	void AimStart();

	/** The character stops aiming */
	void AimStop();

	/** The character starts shooting */
	void FireStart();

	/** The character stops shooting */
	void FireStop();

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn unterface
};
