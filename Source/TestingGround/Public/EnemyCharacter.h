// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TargetPoint.h"
#include "CharacterBase.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_UCLASS_BODY()

	/** An overlap sphere used by AIControllers to check for nearby pawns */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TSubobjectPtr<USphereComponent> AggroTrigger;

	/** Indicates if the enemy character is Patrolling */
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool bIsPatrolling;

	/** The patrol points of the enemy character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
	TArray<ATargetPoint*> PatrolPoints;
};

