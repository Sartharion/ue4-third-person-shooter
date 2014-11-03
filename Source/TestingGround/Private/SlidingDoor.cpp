

#include "TestingGround.h"
#include "SlidingDoor.h"


ASlidingDoor::ASlidingDoor(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->PrimaryActorTick.bCanEverTick = true;

	this->DoorFrame = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, FName(TEXT("DoorFrame")));
	this->DoorFrame->Mobility = EComponentMobility::Static;
	this->RootComponent = this->DoorFrame;

	this->LeftWing = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, FName(TEXT("DoorLeftWing")));
	this->LeftWing->AttachTo(this->RootComponent);
	this->LeftWing->Mobility = EComponentMobility::Movable;

	this->RightWing = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, FName(TEXT("DoorRightWing")));
	this->RightWing->AttachTo(this->RootComponent);
	this->RightWing->Mobility = EComponentMobility::Movable;

	this->TriggerVolume = PCIP.CreateDefaultSubobject<UBoxComponent>(this, FName(TEXT("TriggerVolume")));
	this->TriggerVolume->AttachTo(this->RootComponent);
	this->TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ASlidingDoor::OnDoorTriggerBeginOverlap);
	this->TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ASlidingDoor::OnDoorTriggerEndOverlap);
	this->TriggerVolume->Mobility = EComponentMobility::Movable;
	this->TriggerVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	this->TriggerVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	this->SlideSpeed = 4.0f;
	this->DelayBeforeClosing = 2.0f;
	this->bDoorMustOpen = false;
	this->bDoorMustClose = false;
	this->bUseSlideVectors = true;
	this->bIsActive = true;
	this->LeftWingSlideVector = FVector(0.0f, 0.0f, 0.0f);
	this->RightWingSlideVector = FVector(0.0f, 0.0f, 0.0f);
}

void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (this->bUseSlideVectors)
	{
		this->LeftWingStartLocation = this->LeftWing->GetComponentLocation();
		this->LeftWingEndLocation = this->LeftWingStartLocation + this->LeftWingSlideVector;
		this->RightWingStartLocation = this->RightWing->GetComponentLocation();
		this->RightWingEndLocation = this->RightWingStartLocation + this->RightWingSlideVector;
	}
}

void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!this->bIsActive)
	{
		return;
	}

	if (this->bDoorMustOpen)
	{
		if (this->LeftWing->StaticMesh != NULL)
		{
			if (FVector::DistSquared(this->LeftWing->GetComponentLocation(), this->LeftWingEndLocation) > 1.0f)
			{
				const FVector LeftWingNewLocation = FMath::Lerp(
					this->LeftWing->GetComponentLocation(), this->LeftWingEndLocation, this->SlideSpeed * DeltaTime);

				this->LeftWing->SetWorldLocation(LeftWingNewLocation);
			}
			else
			{
				this->bDoorMustOpen = false;
			}
		}

		if (this->RightWing->StaticMesh != NULL)
		{
			if (FVector::DistSquared(this->RightWing->GetComponentLocation(), this->RightWingEndLocation) > 1.0f)
			{
				const FVector RightWingNewLocation = FMath::Lerp(
					this->RightWing->GetComponentLocation(), this->RightWingEndLocation, this->SlideSpeed * DeltaTime);

				this->RightWing->SetWorldLocation(RightWingNewLocation);
			}
			else
			{
				this->bDoorMustOpen = false;
			}
		}
	}
	else if (this->bDoorMustClose)
	{
		this->DelayBeforeClosingCounter -= DeltaTime;

		if (this->LeftWing->StaticMesh != NULL)
		{
			if (FVector::DistSquared(this->LeftWing->GetComponentLocation(), this->LeftWingStartLocation) > 1.0f)
			{
				if (this->DelayBeforeClosingCounter <= 0.0f)
				{
					const FVector LeftWingNewLocation = FMath::Lerp(
						this->LeftWing->GetComponentLocation(), this->LeftWingStartLocation, this->SlideSpeed * DeltaTime);

					this->LeftWing->SetWorldLocation(LeftWingNewLocation);
				}
			}
			else
			{
				this->bDoorMustClose = false;
			}
		}

		if (this->RightWing->StaticMesh != NULL)
		{
			if (FVector::DistSquared(this->RightWing->GetComponentLocation(), this->RightWingStartLocation) > 1.0f)
			{
				if (this->DelayBeforeClosingCounter <= 0.0f)
				{
					const FVector RightWingNewLocation = FMath::Lerp(
						this->RightWing->GetComponentLocation(), this->RightWingStartLocation, this->SlideSpeed * DeltaTime);

					this->RightWing->SetWorldLocation(RightWingNewLocation);
				}
			}
			else
			{
				this->bDoorMustClose = false;
			}
		}
	}
}

void ASlidingDoor::OnDoorTriggerBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (this->LeftWing->StaticMesh != NULL || this->RightWing->StaticMesh != NULL)
	{
		this->bDoorMustOpen = true;
		this->bDoorMustClose = false;
	}
}

void ASlidingDoor::OnDoorTriggerEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (this->LeftWing->StaticMesh != NULL || this->RightWing->StaticMesh != NULL)
	{
		this->DelayBeforeClosingCounter = this->DelayBeforeClosing;
		this->bDoorMustClose = true;
	}
}
