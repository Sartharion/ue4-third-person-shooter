

#pragma once

#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

	/** The speed of the character in centimeters/sec */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float Speed;

	/**
	 * The direction of the character in degrees in range [-180, 180].
	 * 0 -> Forward
	 * -90 -> Left
	 * 90 -> Right
	 * +-180 -> Backward
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float Direction;

	/** Indicates if the character is aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsAiming;

	/** Indicates if the character is firing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsFiring;

	virtual void BlueprintUpdateAnimation(float DeltaTimeX) override;
};
