// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterBase.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API UCharacterAnimInstanceBase : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

	/** The PawnOwner of this AnimInstance */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	ACharacterBase* Character;

	/** The speed of the character in centimeters/sec */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	float Speed;

	/**
	* The direction of the character in degrees in range [-180, 180].
	* 0 -> Forward
	* -90 -> Left
	* 90 -> Right
	* +-180 -> Backward
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	float Direction;

	/** The Pitch value of the character's Control Rotation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	float Pitch;

	/** Indicates if the character is aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	bool bIsAiming;

	/** Indicates if the character is firing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	bool bIsFiring;

	/** Indicates if the character is reloading */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	bool bIsReloading;

	/** Indicates if the character is dead */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	bool bIsDead;

	virtual void BlueprintUpdateAnimation(float DeltaTimeX) override;

	virtual void BlueprintInitializeAnimation() override;	
};
