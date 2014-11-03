// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TargetPoint.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float FieldOfView;

	/** The controlled enemy character */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	AEnemyCharacter* ControlledCharacter;

	/** The time that the enemy character waits at each patrol point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float WaitTimeAtPatrolPoint;

	/** The time that the enemy character waits at the last know target location after a chase */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	float WaitTimeAfterChase;

	/** Indicates if the target is in range */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target")
	bool bIsTargetInRange;

	/** Indicates if the target is in line of sight */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target")
	bool bIsTargetInLineOfSight;

	/**
	 * The enemy chases a specified target. The enemy will chase the target only if the target is in line of sight
	 * @param Target - the target to chase
	 * @param AcceptanceRadius - the enemy will stop chasing if it reaches a specified AcceptanceRadius
	 * @return "True" if the enemy character is chasing the target, "False" otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	bool ChaseTarget(ACharacterBase* Target, float AcceptanceRadius);

	/**
	 * The enemy starts shooting the target if the target is in line of sight and is in range.
	 * @param Target - the traget to shoot at
	 * @return True if the target was shot, False otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	bool ShootTarget(ACharacterBase* Target);

	/**
	 * The enemy character starts patrolling.
	 * @param PatrolPoints - the target points that form the patrol path
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	void Patrol(const TArray<ATargetPoint*>& PatrolPoints);

	/**
	 * The respond of the enemy character when is hit but is not aware of the target's location
	 * @param Hit - The hit
	 * @param DamageCauser - The Actor that caused the damage (a projectile for example)
	 */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	void RespondToUnawareHit(const FHitResult& Hit, const AActor* DamageCauser);

	/** Stops all of the actions the enemy character is currently doing */
	UFUNCTION(BlueprintCallable, Category = "Character Action")
	void StopAllActions();

	/**
	 * Checks if a specified target is in the line of sight of the EnemyCharacter.
	 * @param Target - the target to be checked
	 * @return True if the target is in line of sight, False otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Target")
	bool IsTargetInLineOfSight(AActor* Target) const;

	/**
	 * Checks if the a specified target is in range
	 * @param AcceptanceRadius - the radius for which the target will be checked if is in range
	 * @return True if the target is in range, False otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Target")
	bool IsTargetInRange(AActor* Target, float AcceptanceRadius) const;

	/** Gets the current followed target */
	UFUNCTION(BlueprintCallable, Category = "Target")
	AActor* GetTarget() const;

	/** Gets the last known location of the followed target */
	UFUNCTION(BlueprintCallable, Category = "Target")
	FVector GetTargetLastKnownLocation() const;

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

protected:
	/** The current target to follow */
	ACharacterBase* Target;

	/** The last known location of the followed target */
	FVector TargetLocation;

	/** 
	 * The counter (in seconds) that determines when the enemy character should go to the next patrol point.
	 * When this counter reaches the WaitTimeAtPatrolPoint (in seconds), then the enemy character start moving to the next patrol point.
	 */
	float WaitTimeAtPatrolPointCounter;

	/**
	 * The counter (in seconds) that determines when the enemy character should start doing his things after a target chase finished.
	  When this counter reaches the WaitTimeAfterChase (in seconds), then the enemy character start doing hit things again.
	 */
	float WaitTimeAfterChaseCounter;

	/** The index of the patrol point where the enemy character needs to move to */
	int32 PatrolPoint;

	/**
	 * The character waits a specified amount of time (in seconds)
	 * @param SecondsToWait - the seconds to wait
	 * @param SecondsCounter - the seconds counter
	 * @return "True" if the character is still waiting, "False" otherwise
	 */
	bool Wait(float SecondsToWait, float& SecondsCounter);

	/** An event called when the AggroTrigger of the EnemyCharacter begins overlap */
	UFUNCTION()
	virtual void OnAggroTriggerBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** An event called when the AggroTrigger of the EnemyCharacter ends overlap */
	UFUNCTION()
	virtual void OnAggroTriggerEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
