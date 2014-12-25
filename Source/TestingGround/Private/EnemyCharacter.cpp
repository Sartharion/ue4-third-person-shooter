// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.h"


AEnemyCharacter::AEnemyCharacter(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	this->AggroTrigger = PCIP.CreateDefaultSubobject<USphereComponent>(this, FName(TEXT("AggroTrigger")));
	this->AggroTrigger->AttachTo(this->RootComponent);
	this->AggroTrigger->InitSphereRadius(1500.0f); // By default the aggro trigger has a radius of 15 meters

	this->bIsPatrolling = true;
}

void AEnemyCharacter::TakeDamage(float Damage, const FHitResult& Hit, const AActor* DamageCauser)
{
	Super::TakeDamage(Damage, Hit, DamageCauser);

	AEnemyAIController* AIController = Cast<AEnemyAIController>(this->Controller);
	if (AIController != NULL)
	{
		AIController->RespondToUnawareHit(Hit, DamageCauser);
	}
}

