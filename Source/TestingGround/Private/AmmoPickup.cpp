

#include "TestingGround.h"
#include "AmmoPickup.h"


AAmmoPickup::AAmmoPickup(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->Ammo = 30;
	this->PickUpAmmoMethodPtr = &AMainCharacter::PickUpAmmo;
}

void AAmmoPickup::OnPickedUp(APawn* Picker)
{
	Super::OnPickedUp(Picker);

	AMainCharacter* Character = Cast<AMainCharacter>(Picker);
	if (Character != NULL && this->PickUpAmmoMethodPtr != NULL)
	{
		int32 PickedUpAmount = (Character->*PickUpAmmoMethodPtr)(this->Ammo);

		if (PickedUpAmount == this->Ammo)
		{
			this->Destroy();
		}
		else
		{
			this->Ammo -= PickedUpAmount;
		}

		if (PickedUpAmount != 0 && this->PickupSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, this->PickupSound, this->GetActorLocation());
		}
	}
}

