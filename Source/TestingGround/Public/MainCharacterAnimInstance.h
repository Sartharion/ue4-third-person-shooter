

#pragma once

#include "Animation/AnimInstance.h"
#include "MainCharacter.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

	/** The PawnOwner of this AnimInstance */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	AMainCharacter* Character;

	/** The speed of the character in centimeters/sec */
	UPROPERTY(BlueprintReadOnly, Category = "State")
	float Speed;

	/**
	 * The direction of the character in degrees in range [-180, 180].
	 * 0 -> Forward
	 * -90 -> Left
	 * 90 -> Right
	 * +-180 -> Backward
	 */
	UPROPERTY(BlueprintReadOnly, Category = "State")
	float Direction;

	/** Indicates if the character is aiming */
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsAiming;

	/** Indicates if the character is firing */
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsFiring;

	/** Indicates if the character is reloading */
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsReloading;

	virtual void BlueprintUpdateAnimation(float DeltaTimeX) override;

	virtual void BlueprintInitializeAnimation() override;
};
