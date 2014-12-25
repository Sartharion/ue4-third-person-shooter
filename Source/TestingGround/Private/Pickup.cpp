

#include "TestingGround.h"
#include "Pickup.h"


APickup::APickup(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	this->PrimaryActorTick.bCanEverTick = true;

	// Initialize the collision component
	this->BoxComponent = PCIP.CreateDefaultSubobject<UBoxComponent>(this, FName(TEXT("BoxCollisionComponent")));
	this->BoxComponent->bCanEverAffectNavigation = false;
	this->BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	this->BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	this->BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	this->RootComponent = this->BoxComponent;

	// Initialize the static mesh component
	this->PickupMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, FName(TEXT("PickupMesh")));
	this->PickupMesh->AttachTo(this->RootComponent);
	this->PickupMesh->bAbsoluteScale = true;
	this->PickupMesh->bCanEverAffectNavigation = false;

	this->bIsActive = true; // The Pickup can be picked up be default
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

