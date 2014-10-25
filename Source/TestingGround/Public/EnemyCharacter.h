// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
};

