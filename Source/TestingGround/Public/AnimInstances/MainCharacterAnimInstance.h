

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

	virtual void BlueprintUpdateAnimation(float DeltaTimeX) override;
};
