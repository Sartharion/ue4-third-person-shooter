

#include "TestingGround.h"
#include "Pickup.h"


APickup::APickup(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->PrimaryActorTick.bCanEverTick = true;

	// Initialize the collision component
	this->SphereComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, FName(TEXT("SphereCollisionComponent")));
	this->SphereComponent->InitSphereRadius(25.0f);

	this->RootComponent = this->SphereComponent;

	// Initialize the static mesh component
	this->PickupMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, FName(TEXT("PickupMesh")));
	this->PickupMesh->AttachTo(this->RootComponent);

	this->bIsActive = true; // The Pickup can be picked up be default
	this->RotationRate = FRotator(30.0f, 45.0f, 60.0f); // The rotation rate at which the pickup will rotate
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->AddActorLocalRotation(this->RotationRate * DeltaTime);
}

void APickup::ReceiveActorBeginOverlap(AActor* OtherActor)
{
	Super::ReceiveActorBeginOverlap(OtherActor);

	APawn* Picker = Cast<APawn>(OtherActor);
	if (Picker != NULL)
	{
		this->OnPickedUp(Picker);
	}
}

void APickup::OnPickedUp(APawn* Picker)
{

}

