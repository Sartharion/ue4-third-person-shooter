// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyCharacter.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AEnemyAIController : public AAIController
{
	GENERATED_UCLASS_BODY()

	/** The field of view of the enemy in degrees */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float FieldOfView;

	/** The controlled enemy character */
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	AEnemyCharacter* ControlledCharacter;

	/** The delay before the enemy goes to his home location in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float DelayBeforeGoingToHomeLocation;

	/**
	 * The enemy chases a specified target.
	 * @param Target - the target to chase
	 * @param AcceptanceRadius - the enemy will stop chasing if it reaches a specified AcceptanceRadius
	 * @param bIsTargetInLineOfSight - the enemy will chase the target only if the target is in line of sight
	 * @param DeltaTime - the time between 2 frames
	 */
	UFUNCTION(BlueprintCallable, Category = "Enemy Action")
	void ChaseTarget(ACharacterBase* Target, float AcceptanceRadius, bool bIsTargetInLineOfSight, float DeltaTime);

	/**
	 * The enemy starts shooting the target if the target is in line of sight and is close enough.
	 * @param Target - the traget to shoot at
	 * @param bIsTargetInLineOfSight - is the target in line of sight
	 * @param bIsTargetCloseEnough - is the target close enough
	 */
	UFUNCTION(BlueprintCallable, Category = "Enemy Action")
	void ShootTarget(ACharacterBase* Target, bool bIsTargetInLineOfSight, bool bIsTargetCloseEnough);

	/**
	 * Checks if a specified target is in the line of sight of the EnemyCharacter.
	 * @param Target - the target to be checked
	 * @return True if the target is in line of sight, False otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Target")
	bool IsTargetInLineOfSight(AActor* Target) const;

	/**
	 * Checks if the a specified target is close enough
	 * @param AcceptanceRadius - the radius for which the target will be checked if is close enough
	 * @return True if the target is close enough, Flase otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Target")
	bool IsTargetCloseEnough(AActor* Target, float AcceptanceRadius) const;

	/** Gets the current followed target */
	UFUNCTION(BlueprintCallable, Category = "Target")
	AActor* GetTarget() const;

	/** Gets the last known location of the followed target */
	UFUNCTION(BlueprintCallable, Category = "Target")
	FVector GetTargetLastKnownLocation() const;

	/** The location to where the enemy character will go when the target to follow is lost */
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	FVector GetHomeLocation() const;

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

protected:
	/** The current target to follow */
	ACharacterBase* Target;

	/** The last known location of the followed target */
	FVector TargetLocation;

	/** The location to where the enemy will go when the target to follow is lost */
	FVector HomeLocation;

	/** Indicates if the enemy character should to it's HomeLocation */
	bool bShouldMoveToHomeLocation;

	/** 
	 * The counter that determines when the enemy should go to his home location.
	 * When this counter reaches the DelayBeforeGoingToHomeLocation, then the enemy start moving to the home location.
	 */
	float DelayBeforeGoingToHomeLocationCounter;

	/** An event called when the AggroTrigger of the EnemyCharacter begins overlap */
	UFUNCTION()
	virtual void OnAggroTriggerBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** An event called when the AggroTrigger of the EnemyCharacter ends overlap */
	UFUNCTION()
	virtual void OnAggroTriggerEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
