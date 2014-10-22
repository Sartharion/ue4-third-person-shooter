

#include "TestingGround.h"
#include "SlidingDoor.h"


ASlidingDoor::ASlidingDoor(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->PrimaryActorTick.bCanEverTick = true;

	this->TriggerVolume = PCIP.CreateDefaultSubobject<UBoxComponent>(this, FName(TEXT("TriggerVolume")));
	this->RootComponent = this->TriggerVolume;

	this->DoorMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, FName(TEXT("DoorMesh")));
	this->DoorMesh->bAbsoluteScale = true;

	this->SlideSpeed = 4.0f;
	this->SlideVector = FVector(0.0f, 0.0f, 0.0f);
	this->SlidingDoorCloseDelay = 2.0f;
	this->bInitializedDoor = false;
	this->bDoorMustOpen = false;
	this->bDoorMustClose = false;
}

void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->DoorMesh && this->bDoorMustOpen)
	{
		if (FVector::Dist(this->DoorMesh->GetComponentLocation(), this->SlidingDoorEndLocation) > 1.0f)
		{
			FVector NewLocation = FMath::Lerp(
				this->DoorMesh->GetComponentLocation(),
				this->SlidingDoorEndLocation,
				this->SlideSpeed * DeltaTime);

			this->DoorMesh->SetWorldLocation(NewLocation);
		}
		else
		{
			this->bDoorMustOpen = false;
		}
	}
	else if (this->DoorMesh && this->bDoorMustClose)
	{
		this->SlidingDoorCloseDelaySecondsCounter -= DeltaTime;

		if (FVector::Dist(this->DoorMesh->GetComponentLocation(), this->SlidingDoorStartLocation) > 1.0f)
		{
			if (this->SlidingDoorCloseDelaySecondsCounter <= 0.0f)
			{
				FVector NewLocation = FMath::Lerp(
					this->DoorMesh->GetComponentLocation(),
					this->SlidingDoorStartLocation,
					this->SlideSpeed * DeltaTime);

				this->DoorMesh->SetWorldLocation(NewLocation);
			}
		}
		else
		{
			this->bDoorMustClose = false;
		}
	}
}

void ASlidingDoor::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	Super::ReceiveActorBeginOverlap(OtherActor);

	if (this->DoorMesh)
	{
		if (!this->bInitializedDoor)
		{
			this->SlidingDoorStartLocation = this->DoorMesh->GetComponentLocation();
			this->SlidingDoorEndLocation = this->SlidingDoorStartLocation + this->SlideVector;

			this->bInitializedDoor = true;
		}

		this->bDoorMustOpen = true;
		this->bDoorMustClose = false;
	}
}

void ASlidingDoor::ReceiveActorEndOverlap(AActor* OtherActor)
{
	Super::ReceiveActorEndOverlap(OtherActor);

	if (this->DoorMesh)
	{
		this->SlidingDoorCloseDelaySecondsCounter = this->SlidingDoorCloseDelay;
		this->bDoorMustClose = true;
	}
}
