// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "EnemyAIController.h"
#include "MainCharacter.h"


AEnemyAIController::AEnemyAIController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->FieldOfView = 150.0f; // The FOV of the enemy character in degrees
	this->Target = NULL;
	this->TargetLocation = FVector::ZeroVector;
	this->PatrolPoint = 0;
	this->WaitTimeAtPatrolPoint = 5.0f; // In seconds
	this->WaitTimeAtPatrolPointCounter = 0.0f; // In seconds
	this->WaitTimeAfterChase = 5.0f; // In seconds
	this->WaitTimeAfterChaseCounter = 0.0f; // In seconds
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* ControlledPawn = this->GetPawn();
	if (ControlledPawn != NULL)
	{
		this->ControlledCharacter = Cast<AEnemyCharacter>(ControlledPawn);

		if (this->ControlledCharacter != NULL)
		{
			this->ControlledCharacter->AggroTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAIController::OnAggroTriggerBeginOverlap);
			this->ControlledCharacter->AggroTrigger->OnComponentEndOverlap.AddDynamic(this, &AEnemyAIController::OnAggroTriggerEndOverlap);
		}
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->ControlledCharacter == NULL || this->ControlledCharacter->bIsDead)
	{
		return;
	}

	if (this->Target != NULL)
	{
		float AcceptanceRadius = this->ControlledCharacter->AggroTrigger->GetUnscaledSphereRadius() * 0.7f;
		bool bIsTargetCloseEnough = this->IsTargetCloseEnough(this->Target, AcceptanceRadius);
		bool bIsTargetInLineOfSight = this->IsTargetInLineOfSight(this->Target);

		this->ShootTarget(this->Target, bIsTargetInLineOfSight, bIsTargetCloseEnough);

		if (!this->ChaseTarget(this->Target, 100.0f, bIsTargetInLineOfSight))
		{
			// If the chase is not successful, then patrol
			this->Patrol(this->ControlledCharacter->PatrolPoints);
		}
	}
	else
	{
		this->Patrol(this->ControlledCharacter->PatrolPoints);
	}
}

bool AEnemyAIController::ChaseTarget(ACharacterBase* Target, float AcceptanceRadius, bool bIsTargetInLineOfSight)
{
	bool bIsChasingTarget = false;

	if (bIsTargetInLineOfSight && !Target->bIsDead)
	{
		bIsChasingTarget = true;

		if (!this->ControlledCharacter->bIsSprinting)
		{
			this->ControlledCharacter->SprintStart();
		}

		this->ControlledCharacter->bIsPatrolling = false;
		this->TargetLocation = Target->GetActorLocation();
		this->MoveToLocation(this->TargetLocation, AcceptanceRadius);
	}
	else if (!this->ControlledCharacter->bIsPatrolling && !Target->bIsDead)
	{
		bIsChasingTarget = true;

		EPathFollowingRequestResult::Type PathRequestResult = this->MoveToLocation(this->TargetLocation, AcceptanceRadius);
		if (PathRequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			bool bStillWaiting = this->Wait(this->WaitTimeAfterChase, this->WaitTimeAfterChaseCounter);
			if (!bStillWaiting)
			{
				bIsChasingTarget = false;
				this->ControlledCharacter->bIsPatrolling = true;
			}
		}
	}

	return bIsChasingTarget;
}

void AEnemyAIController::ShootTarget(ACharacterBase* Target, bool bIsTargetInLineOfSight, bool bIsTargetCloseEnough)
{
	if (this->ControlledCharacter->AmmoInClip == 0 &&
		!this->ControlledCharacter->bIsReloading)
	{
		if (this->ControlledCharacter->bIsFiring)
		{
			this->ControlledCharacter->FireStop();
			this->ControlledCharacter->AimStop();
		}

		this->ControlledCharacter->ReloadStart();

		return;
	}

	if (this->Target == NULL || this->Target->bIsDead)
	{
		if (this->ControlledCharacter->bIsFiring)
		{
			this->ControlledCharacter->FireStop();
			this->ControlledCharacter->AimStop();
		}

		return;
	}

	if (bIsTargetInLineOfSight && bIsTargetCloseEnough)
	{
		// Update the control rotation so that the enemy character fires at correct direction
		const FVector AimDirection = Target->GetActorLocation() - this->ControlledCharacter->GetActorLocation();
		const FRotator AimControlRotation = FRotationMatrix::MakeFromX(AimDirection).Rotator();
		this->ControlledCharacter->Controller->SetControlRotation(AimControlRotation);

		// Update the yaw rotation of the enemy character so that when he is firing, he is facing his target
		FRotator CharacterRotation = this->ControlledCharacter->GetActorRotation();
		CharacterRotation.Yaw = AimControlRotation.Yaw;
		this->ControlledCharacter->SetActorRotation(CharacterRotation);

		// Start firing
		if (!this->ControlledCharacter->bIsFiring &&
			!this->ControlledCharacter->bIsReloading)
		{
			this->ControlledCharacter->AimStart();
			this->ControlledCharacter->FireStart();
		}
	}
	else
	{
		if (this->ControlledCharacter->bIsFiring)
		{
			this->ControlledCharacter->FireStop();
			this->ControlledCharacter->AimStop();
		}
	}
}

void AEnemyAIController::Patrol(const TArray<ATargetPoint*>& PatrolPoints)
{
	if (PatrolPoints.Num() > 0)
	{
		if (this->ControlledCharacter->bIsSprinting)
		{
			this->ControlledCharacter->SprintStop();
		}

		if (this->ControlledCharacter->bIsFiring)
		{
			this->ControlledCharacter->FireStop();
			this->ControlledCharacter->AimStop();
		}

		ATargetPoint* PatrolPointActor = PatrolPoints[this->PatrolPoint];
		EPathFollowingRequestResult::Type PathRequestResult = this->MoveToActor(PatrolPointActor);
		if (PathRequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			bool bStillWaiting = this->Wait(this->WaitTimeAtPatrolPoint, this->WaitTimeAtPatrolPointCounter);
			if (!bStillWaiting)
			{
				this->PatrolPoint = (this->PatrolPoint + 1) % PatrolPoints.Num();
			}
		}		
	}
}

void AEnemyAIController::StopAllActions()
{
	this->ControlledCharacter->FireStop();
	this->ControlledCharacter->AimStop();
	this->ControlledCharacter->SprintStop();
	this->ControlledCharacter->ReloadStop();
}

bool AEnemyAIController::IsTargetInLineOfSight(AActor* Target) const
{
	bool bIsInLineOfSight = false;

	if (Target != NULL)
	{
		UWorld* World = this->GetWorld();
		if (World != NULL)
		{
			// Make a raycast to the Target to see if the it is visible
			const FVector RayStart = this->ControlledCharacter->GetActorLocation() + FVector(0.0f, 0.0f, this->ControlledCharacter->BaseEyeHeight);
			const FVector RayEnd = Target->GetActorLocation();
			FCollisionQueryParams QueryParams(FName(TEXT("TargetVisibility")), false, this);
			QueryParams.AddIgnoredActor(this->ControlledCharacter);
			FHitResult HitResult;

			if (World->LineTraceSingle(HitResult, RayStart, RayEnd, ECollisionChannel::ECC_Camera, QueryParams))
			{
				if (HitResult.Actor == Target)
				{
					// The target is visible, but we need to check if the target is in the line of sight of the enemy character
					FVector EnemyForwardVector = this->ControlledCharacter->GetActorForwardVector();
					FVector DirectionToTarget = Target->GetActorLocation() - this->ControlledCharacter->GetActorLocation();

					EnemyForwardVector.Normalize();
					DirectionToTarget.Normalize();

					float AngleBetween = FMath::Acos(FVector::DotProduct(EnemyForwardVector, DirectionToTarget));
					AngleBetween = FMath::RadiansToDegrees(AngleBetween);

					if (AngleBetween <= this->FieldOfView * 0.5f)
					{
						bIsInLineOfSight = true;
					}
				}
			}
		}
	}

	return bIsInLineOfSight;
}

bool AEnemyAIController::IsTargetCloseEnough(AActor* Target, float AcceptanceRadius) const
{
	bool bIsCloseEnough = (Target->GetActorLocation() - this->ControlledCharacter->GetActorLocation()).Size() <= AcceptanceRadius;

	return bIsCloseEnough;
}

AActor* AEnemyAIController::GetTarget() const
{
	return this->Target;
}

FVector AEnemyAIController::GetTargetLastKnownLocation() const
{
	return this->TargetLocation;
}

bool AEnemyAIController::Wait(float SecondsToWait, float& SecondsCounter)
{
	bool bStillWaiting = true;

	SecondsCounter += this->GetWorld()->GetDeltaSeconds();
	if (SecondsCounter >= SecondsToWait)
	{
		bStillWaiting = false;
		SecondsCounter = 0.0f;
	}

	return bStillWaiting;
}

void AEnemyAIController::OnAggroTriggerBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter != NULL)
	{
		this->Target = MainCharacter;
	}
}

void AEnemyAIController::OnAggroTriggerEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter != NULL)
	{
		this->Target = NULL;
	}
}

