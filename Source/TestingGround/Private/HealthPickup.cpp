// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "HealthPickup.h"
#include "MainCharacter.h"


AHealthPickup::AHealthPickup(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	this->Health = 500.0f;
}

void AHealthPickup::OnPickedUp(APawn* Picker)
{
	Super::OnPickedUp(Picker);

	AMainCharacter* Character = Cast<AMainCharacter>(Picker);
	if (Character != NULL)
	{
		float GainedHealthAmount = Character->GainHealth(this->Health);

		if (GainedHealthAmount == this->Health)
		{
			this->Destroy();
		}
		else
		{
			this->Health -= GainedHealthAmount;
		}

		if (GainedHealthAmount != 0 && this->PickupSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, this->PickupSound, this->GetActorLocation());
		}
	}
}
