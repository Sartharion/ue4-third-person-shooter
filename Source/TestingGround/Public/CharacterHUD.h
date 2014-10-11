

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

	UPROPERTY(EditDefaultsOnly, Category = "Font")
	UFont* Font;

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

private:
	AMainCharacter* Character;
};
