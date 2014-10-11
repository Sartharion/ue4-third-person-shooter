

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
		// Draw the remaining ammo
		FVector2D AmmoPosition = FVector2D(Canvas->ClipX - Canvas->ClipX / 6.0f, Canvas->ClipY - Canvas->ClipY / 7.0f);
		FText Ammo = FText::FromString(FString(TEXT("Ammo: ") + FString::FromInt(this->Character->Ammo)));
		FCanvasTextItem AmmoTextItem(AmmoPosition, Ammo, this->Font, FLinearColor::White);
		Canvas->DrawItem(AmmoTextItem);

		// Draw the remaining ammo in the clip of the gun
		FVector2D ClipPosition = FVector2D(Canvas->ClipX - Canvas->ClipX / 6.0f, Canvas->ClipY - Canvas->ClipY / 10.0f);
		FText Clip = FText::FromString(FString(TEXT("Clip: ") + FString::FromInt(this->Character->AmmoInClip)));
		FCanvasTextItem ClipTextItem(ClipPosition, Clip, this->Font, FLinearColor::White);
		Canvas->DrawItem(ClipTextItem);
	}
}
