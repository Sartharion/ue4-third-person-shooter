

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
		this->DrawCrosshair();
	}
}

void ACharacterHUD::DrawAmmoHUD()
{
	if (this->Font != NULL)
	{
		// Draw the remaining ammo
		if (this->Character->EquippedWeapon != NULL)
		{
			FVector2D AmmoPosition(Canvas->ClipX - Canvas->ClipX / 8.0f, Canvas->ClipY - Canvas->ClipY / 7.0f);
			FText AmmoText = FText::FromString(FString::Printf(TEXT("Ammo: %d"), this->Character->EquippedWeapon->RemainingAmmo));
			FCanvasTextItem AmmoTextItem(AmmoPosition, AmmoText, this->Font, FLinearColor::White);
			Canvas->DrawItem(AmmoTextItem);

			// Draw the remaining ammo in the clip of the gun
			FVector2D ClipPosition(Canvas->ClipX - Canvas->ClipX / 8.0f, Canvas->ClipY - Canvas->ClipY / 10.0f);
			FText ClipText = FText::FromString(FString::Printf(TEXT("Clip: %d"), this->Character->EquippedWeapon->AmmoInClip));
			FCanvasTextItem ClipTextItem(ClipPosition, ClipText, this->Font, FLinearColor::White);
			Canvas->DrawItem(ClipTextItem);
		}
	}
}

void ACharacterHUD::DrawHealthBarHUD()
{
	if (this->Font != NULL)
	{
		// Draw the health bar of the character
		FVector2D HealthBarPosition(25.0f, Canvas->ClipY - Canvas->ClipY / 10.0f);
		FText HealthBarText = FText::FromString(FString::Printf(TEXT("Health: %.0f/%.0f"), Character->Health, Character->HealthCapacity));
		FCanvasTextItem HealthBarTextItem(HealthBarPosition, HealthBarText, this->Font, FLinearColor::White);
		Canvas->DrawItem(HealthBarTextItem);
	}
}

void ACharacterHUD::DrawCrosshair()
{
	if (this->CrosshairTexture != NULL && this->Character->bIsAiming)
	{
		FVector2D Center(Canvas->ClipX / 2.0f, Canvas->ClipY / 2.0f);

		FVector2D CrosshairPosition(
			Center.X - (CrosshairTexture->GetSurfaceWidth() / 2.0f),
			Center.Y - (CrosshairTexture->GetSurfaceHeight() / 2.0f));

		FCanvasTileItem CrosshairTile(CrosshairPosition, CrosshairTexture->Resource, FLinearColor::White);
		CrosshairTile.BlendMode = SE_BLEND_Translucent;

		Canvas->DrawItem(CrosshairTile);
	}
}

