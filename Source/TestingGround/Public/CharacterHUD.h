

#pragma once

#include "GameFramework/HUD.h"
#include "MainCharacter.h"
#include "CharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API ACharacterHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

	/** The font use for the text */
	UPROPERTY(EditDefaultsOnly, Category = "Text")
	UFont* Font;

	/** The texture for the crosshair */
	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	UTexture2D* CrosshairTexture;

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

private:
	AMainCharacter* Character;

	void DrawAmmoHUD();

	void DrawHealthBarHUD();

	void DrawCrosshair();
};
