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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	AEnemyCharacter* ControlledCharacter;

	/** Checks if the target to follow is in the line of sight of the EnemyCharacter */
	UFUNCTION(BlueprintCallable, Category = "Target")
	bool IsTargetToFollowInLineOfSight() const;

	/** Gets the current followed target */
	UFUNCTION(BlueprintCallable, Category = "Target")
	AActor* GetTargetToFollow() const;

	/** Gets the last known location of the followed target */
	UFUNCTION(BlueprintCallable, Category = "Target")
	FVector GetTargetLastKnownLocation() const;

	/** The location to where the enemy character will go when the target to follow is lost */
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	FVector GetHomeLocation() const;

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

protected:
	/** The location to where the enemy will go when the target to follow is lost */
	FVector HomeLocation;

	/** The current target to follow */
	AActor* TargetToFollow;

	/** The last known location of the followed target */
	FVector TargetLocation;

	/** An event called when the AggroTrigger of the EnemyCharacter begins overlap */
	UFUNCTION()
	virtual void OnAggroTriggerBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** An event called when the AggroTrigger of the EnemyCharacter ends overlap */
	UFUNCTION()
	virtual void OnAggroTriggerEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
