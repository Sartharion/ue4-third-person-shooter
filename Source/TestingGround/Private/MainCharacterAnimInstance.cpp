

#include "TestingGround.h"
#include "MainCharacterAnimInstance.h"


UMainCharacterAnimInstance::UMainCharacterAnimInstance(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	this->Speed = 0.0f;
	this->Direction = 0.0f;
	this->bIsAiming = false;
	this->bIsFiring = false;
	this->bIsReloading = false;
}

void UMainCharacterAnimInstance::BlueprintInitializeAnimation()
{
	Super::BlueprintInitializeAnimation();

	APawn* PawnOwner = this->TryGetPawnOwner();
	if (PawnOwner != NULL)
	{
		this->Character = Cast<AMainCharacter>(PawnOwner);
	}
}

void UMainCharacterAnimInstance::BlueprintUpdateAnimation(float DeltaTimeX)
{
	Super::BlueprintUpdateAnimation(DeltaTimeX);

	if (this->Character != NULL)
	{
		const FVector CharacterVelocity = this->Character->GetVelocity();
		this->Speed = CharacterVelocity.Size();

		const FRotator CharacterRotation = this->Character->GetActorRotation();
		this->Direction = this->CalculateDirection(CharacterVelocity, CharacterRotation);		

		this->bIsAiming = this->Character->bIsAiming;
		this->bIsFiring = this->Character->bIsFiring;
		this->bIsReloading = this->Character->bIsReloading;

		// Debug messages
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, FString(TEXT("Speed: ")) + FString::SanitizeFloat(this->Speed));
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString(TEXT("Direction: ")) + FString::SanitizeFloat(this->Direction));
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Blue, this->bIsAiming ? FString(TEXT("IsAiming: True")) : FString(TEXT("IsAiming: False")));
		GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Yellow, this->bIsFiring ? FString(TEXT("IsFiring: True")) : FString(TEXT("IsFiring: False")));
		GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Magenta, this->Character->bIsSprinting ? FString(TEXT("IsSprinting: True")) : FString(TEXT("IsSprinting: False")));
		GEngine->AddOnScreenDebugMessage(5, 1.0f, FColor::Cyan, this->bIsReloading ? FString(TEXT("IsReloading: True")) : FString(TEXT("IsReloading: False")));
	}
}
