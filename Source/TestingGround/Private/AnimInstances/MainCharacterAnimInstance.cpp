

#include "TestingGround.h"
#include "MainCharacterAnimInstance.h"


UMainCharacterAnimInstance::UMainCharacterAnimInstance(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->Speed = 0.0f;
}

void UMainCharacterAnimInstance::BlueprintUpdateAnimation(float DeltaTimeX)
{
	Super::BlueprintUpdateAnimation(DeltaTimeX);

	APawn* PawnOwner = this->TryGetPawnOwner();

	if (PawnOwner != NULL)
	{
		this->Speed = PawnOwner->GetVelocity().Size();
	}
}
