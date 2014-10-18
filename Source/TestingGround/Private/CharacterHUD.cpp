

#include "TestingGround.h"
#include "CharacterHUD.h"


ACharacterHUD::ACharacterHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}

void ACharacterHUD::BeginPlay()
{
	Super::BeginPlay();

	if (this->PlayerOwner != NULL)
	{
		this->Character = Cast<AMainCharacter>(this->PlayerOwner->GetCharacter());
	}
}

void ACharacterHUD::DrawHUD()
{
	Super::DrawHUD();

	if (this->Character != NULL)
	{
		this->DrawHealthBarHUD();
		this->DrawAmmoHUD();
	}
}

void ACharacterHUD::DrawAmmoHUD()
{
	// Draw the remaining ammo
	FVector2D AmmoPosition = FVector2D(Canvas->ClipX - Canvas->ClipX / 9.0f, Canvas->ClipY - Canvas->ClipY / 7.0f);
	FText AmmoText = FText::FromString(FString::Printf(TEXT("Ammo: %d"), this->Character->Ammo));
	FCanvasTextItem AmmoTextItem(AmmoPosition, AmmoText, this->Font, FLinearColor::White);
	Canvas->DrawItem(AmmoTextItem);

	// Draw the remaining ammo in the clip of the gun
	FVector2D ClipPosition = FVector2D(Canvas->ClipX - Canvas->ClipX / 9.0f, Canvas->ClipY - Canvas->ClipY / 10.0f);
	FText ClipText = FText::FromString(FString::Printf(TEXT("Clip: %d"), this->Character->AmmoInClip));
	FCanvasTextItem ClipTextItem(ClipPosition, ClipText, this->Font, FLinearColor::White);
	Canvas->DrawItem(ClipTextItem);
}

void ACharacterHUD::DrawHealthBarHUD()
{
	// Draw the health bar of the character
	FVector2D HealthBarPosition = FVector2D(25.0f, Canvas->ClipY - Canvas->ClipY / 10.0f);
	FText HealthBarText = FText::FromString(FString::Printf(TEXT("Health: %.0f/%.0f"), Character->Health, Character->HealthCapacity));
	FCanvasTextItem HealthBarTextItem(HealthBarPosition, HealthBarText, this->Font, FLinearColor::White);
	Canvas->DrawItem(HealthBarTextItem);
}

