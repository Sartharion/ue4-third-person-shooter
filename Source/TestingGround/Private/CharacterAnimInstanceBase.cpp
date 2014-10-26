// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "CharacterAnimInstanceBase.h"


UCharacterAnimInstanceBase::UCharacterAnimInstanceBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->Speed = 0.0f;
	this->Direction = 0.0f;
	this->Pitch = 0.0f;
	this->bIsAiming = false;
	this->bIsFiring = false;
	this->bIsReloading = false;
	this->bIsDead = false;
}

void UCharacterAnimInstanceBase::BlueprintInitializeAnimation()
{
	Super::BlueprintInitializeAnimation();

	APawn* PawnOwner = this->TryGetPawnOwner();
	if (PawnOwner != NULL)
	{
		this->Character = Cast<ACharacterBase>(PawnOwner);
	}
}

void UCharacterAnimInstanceBase::BlueprintUpdateAnimation(float DeltaTimeX)
{
	Super::BlueprintUpdateAnimation(DeltaTimeX);

	if (this->Character != NULL)
	{
		this->bIsDead = this->Character->bIsDead;

		const FVector CharacterVelocity = this->Character->GetVelocity();
		this->Speed = CharacterVelocity.Size();

		const FRotator CharacterRotation = this->Character->GetActorRotation();
		this->Direction = this->CalculateDirection(CharacterVelocity, CharacterRotation);

		this->bIsAiming = this->Character->bIsAiming;
		this->bIsFiring = this->Character->bIsFiring;
		this->bIsReloading = this->Character->bIsReloading;
		this->Pitch = FMath::ClampAngle(this->Character->GetControlRotation().Pitch, -90.0f, 90.0f);

		// Debug messages
		//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, FString::Printf(TEXT("Speed: %.0f"), this->Speed));
		//GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, FString::Printf(TEXT("Direction: %.0f"), this->Direction));
		//GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Blue, FString::Printf(TEXT("Pitch: %.0f"), this->Pitch));
		//GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Yellow, this->bIsAiming ? FString(TEXT("IsAiming: True")) : FString(TEXT("IsAiming: False")));
		//GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Magenta, this->bIsFiring ? FString(TEXT("IsFiring: True")) : FString(TEXT("IsFiring: False")));
		//GEngine->AddOnScreenDebugMessage(5, 1.0f, FColor::Cyan, this->Character->bIsSprinting ? FString(TEXT("IsSprinting: True")) : FString(TEXT("IsSprinting: False")));
		//GEngine->AddOnScreenDebugMessage(6, 1.0f, FColor::Red, this->bIsReloading ? FString(TEXT("IsReloading: True")) : FString(TEXT("IsReloading: False")));
		//GEngine->AddOnScreenDebugMessage(7, 1.0f, FColor::Green, this->bIsDead ? FString(TEXT("IsDead: True")) : FString(TEXT("IsDead: False")));
	}
}
