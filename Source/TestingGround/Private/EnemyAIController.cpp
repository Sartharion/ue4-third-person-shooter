// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "EnemyAIController.h"
#include "MainCharacter.h"


AEnemyAIController::AEnemyAIController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->FieldOfView = 150.0f; // The FOV of the enemy character is degrees
	this->TargetToFollow = NULL;
	this->TargetLocation = FVector::ZeroVector;
	this->bShouldMoveToHomeLocation = true;
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
			this->HomeLocation = this->ControlledCharacter->GetActorLocation();

			this->ControlledCharacter->AggroTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAIController::OnAggroTriggerBeginOverlap);
			this->ControlledCharacter->AggroTrigger->OnComponentEndOverlap.AddDynamic(this, &AEnemyAIController::OnAggroTriggerEndOverlap);
		}
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->TargetToFollow != NULL)
	{
		this->TargetLocation = this->TargetToFollow->GetActorLocation();
		if (this->IsTargetToFollowInLineOfSight())
		{
			this->bShouldMoveToHomeLocation = false;
			this->MoveToLocation(this->TargetLocation, 100.0f);

			/*if (!this->ControlledCharacter->bIsFiring)
			{
				this->ControlledCharacter->AimStart();
				this->ControlledCharacter->FireStart();
			}*/
		}
	}
	else
	{
		this->MoveToLocation(this->HomeLocation, 10.0f);
	}
}

bool AEnemyAIController::IsTargetToFollowInLineOfSight() const
{
	bool bIsInLineOfSight = false;

	if (this->TargetToFollow != NULL)
	{
		UWorld* World = this->GetWorld();
		if (World != NULL)
		{
			// Make a raycast to the TargetToFollow to see if the target is visible
			const FVector RayStart = this->ControlledCharacter->GetActorLocation() + FVector(0.0f, 0.0f, this->ControlledCharacter->BaseEyeHeight);
			const FVector RayEnd = this->TargetLocation;
			FCollisionQueryParams QueryParams(FName(TEXT("TargetToFollowVisibility")), false, this);
			QueryParams.AddIgnoredActor(this->ControlledCharacter);
			FHitResult HitResult;

			if (World->LineTraceSingle(HitResult, RayStart, RayEnd, ECollisionChannel::ECC_Camera, QueryParams))
			{
				if (HitResult.Actor == this->TargetToFollow) 
				{
					// The target is visible, but we need to check if the target is in the line of sight of the enemy character
					FVector EnemyForwardVector = this->ControlledCharacter->GetActorForwardVector();
					FVector DirectionToTarget = this->TargetLocation - this->ControlledCharacter->GetActorLocation();

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

AActor* AEnemyAIController::GetTargetToFollow() const
{
	return this->TargetToFollow;
}

FVector AEnemyAIController::GetTargetLastKnownLocation() const
{
	return this->TargetLocation;
}

FVector AEnemyAIController::GetHomeLocation() const
{
	return this->HomeLocation;
}

void AEnemyAIController::OnAggroTriggerBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter != NULL)
	{
		this->TargetToFollow = MainCharacter;
	}
}

void AEnemyAIController::OnAggroTriggerEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter != NULL)
	{
		this->TargetToFollow = NULL;
	}
}

