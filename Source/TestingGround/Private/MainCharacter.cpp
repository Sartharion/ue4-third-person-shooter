

#include "TestingGround.h"
#include "MainCharacter.h"


AMainCharacter::AMainCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	this->CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, FName(TEXT("CameraBoom")));
	this->CameraBoom->AttachTo(this->RootComponent);
	this->CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character in centimeters
	this->CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the control rotation

	// Create an extension for the CameraBoom. This extension is used of OTS (Over The Shoulder) view
	this->CameraBoomExtension = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, FName(TEXT("CameraBoomExtension")));
	this->CameraBoomExtension->AttachTo(this->CameraBoom, USpringArmComponent::SocketName);
	this->CameraBoomExtension->TargetArmLength = 30.0f; // The camera is this distance right of the target
	this->CameraBoomExtension->bUsePawnControlRotation = false; // Already uses pawn control rotation, because is attached to the CameraBoom
	this->CameraBoomExtension->SetRelativeRotation(FRotator(-10.0f, -90.0f, 0.0f)); // Rotate it -90degrees so that the camera is over the right shoulder of the character

	// Create a follow camera
	this->FollowCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, FName(TEXT("FollowCamera")));
	this->FollowCamera->AttachTo(this->CameraBoomExtension, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	this->FollowCamera->bUsePawnControlRotation = false; // Already uses pawn control rotation, because is attached to the CameraBoom
	this->FollowCamera->SetWorldRotation(FRotator::ZeroRotator);

	// Camera settings
	this->CameraBoomLengthWhileAiming = 100.0f; // The camera is this distance behind the target
	this->CameraBoomExtensionLengthWhileAiming = 50.0f; // The camera is distance right of the target
	this->CameraTransitionSmoothSpeed = 15.0f; // The smooth speed at which the camera transitions between 2 points in space (A multipllier for DeltaTime)
	this->MouseXSensitivity = 1.0f;
	this->MouseYSensitivity = 1.0f;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->CameraBoomLengthCache = this->CameraBoom->TargetArmLength;
	this->CameraBoomExtensionLengthCache = this->CameraBoomExtension->TargetArmLength;
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bIsDead)
	{
		return;
	}

	if (this->bIsAiming)
	{
		this->MoveCameraCloserToCharacter(this->CameraTransitionSmoothSpeed, DeltaTime);
	}
	else
	{
		this->MoveCameraAwayFromCharacter(this->CameraTransitionSmoothSpeed, DeltaTime);
	}
}

void AMainCharacter::Turn(float AxisValue)
{
	this->AddControllerYawInput(AxisValue * this->MouseXSensitivity);
}

void AMainCharacter::LookUp(float AxisValue)
{
	this->AddControllerPitchInput(AxisValue * this->MouseYSensitivity);
}

void AMainCharacter::OnFire()
{
	// Try and fire a projectile
	if (this->RifleProjectileClass != NULL &&
		this->bIsAiming &&
		!this->bIsReloading &&
		this->AmmoInClip > 0)
	{
		UWorld* World = this->GetWorld();
		if (World != NULL)
		{
			// Find the spawn location of the projectile
			FVector SpawnLocation;
			if (this->GunMesh != NULL)
			{
				SpawnLocation = this->GunMesh->GetSocketLocation(this->GunMuzzleSocketName);
			}
			else
			{
				// In case there is no WeaponMesh, then find an alternative SpawnLocation.
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				SpawnLocation = this->GetActorLocation() + this->GetControlRotation().RotateVector(this->GunMuzzleOffset);
			}

			// Find the SpawnRotation of the projectile
			const FRotator CameraRotation = this->FollowCamera->GetComponentRotation();
			const FVector CameraLocation = this->FollowCamera->GetComponentLocation();
			const FVector ForwardVector = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);

			const FVector RayStart = CameraLocation;
			const FVector RayEnd = RayStart + (ForwardVector * 10000);
			FCollisionQueryParams QueryParams(FName(TEXT("ProjectileTrace")), true, this);

			//DrawDebugLine(World, RayStart, RayEnd, FColor::Green, false, 5.0f, 0, 2.0f);
			//DrawDebugLine(World, SpawnLocation, RayEnd, FColor::Blue, false, 5.0f, 2.0f);

			FVector ProjectileDirection = RayEnd - SpawnLocation; // The default projectile direction is from the SpawnLocation to the RayEnd
			FHitResult HitResult;
			if (World->LineTraceSingle(HitResult, RayStart, RayEnd, ECollisionChannel::ECC_Visibility, QueryParams))
			{
				ProjectileDirection = HitResult.Location - SpawnLocation; // If we hit something, we find more accurate projectile direction
				DrawDebugLine(World, SpawnLocation, HitResult.Location, FColor::Red, false, 5.0f, 2.0f);

				AActor* HitActor = HitResult.Actor.Get();
				if (HitActor != NULL)
				{
					GEngine->AddOnScreenDebugMessage(10, 2.0f, FColor::White, HitActor->GetName());
				}
			}

			const FRotator SpawnRotation = FRotationMatrix::MakeFromX(ProjectileDirection).Rotator();

			// Spawn the projectile and reduce the ammo in the clip
			if (this->bIsUsingRifle)
			{
				World->SpawnActor<AProjectileBase>(this->RifleProjectileClass, SpawnLocation, SpawnRotation);
			}

			this->OnFireAnimation();
			this->AmmoInClip--;
		}
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	if (InputComponent != NULL)
	{
		InputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
		InputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
		InputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);

		InputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::SprintStart);
		InputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::SprintStop);
		InputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::AimStart);
		InputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::AimStop);
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::FireStart);
		InputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::FireStop);
		InputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacter::ReloadStart);
	}
}

void AMainCharacter::MoveCameraCloserToCharacter(float TransitionSmoothSpeed, float DeltaTime)
{
	if (FMath::Abs(this->CameraBoom->TargetArmLength - this->CameraBoomLengthWhileAiming) > 1.0f)
	{
		// The camera must get closer to the character
		this->CameraBoom->TargetArmLength =
			FMath::Lerp(this->CameraBoom->TargetArmLength, this->CameraBoomLengthWhileAiming, TransitionSmoothSpeed * DeltaTime);
	}

	if (FMath::Abs(this->CameraBoomExtension->TargetArmLength - this->CameraBoomExtensionLengthWhileAiming) > 1.0)
	{
		// The camera must be at this distance right of the target
		this->CameraBoomExtension->TargetArmLength =
			FMath::Lerp(this->CameraBoomExtension->TargetArmLength, this->CameraBoomExtensionLengthWhileAiming, TransitionSmoothSpeed * DeltaTime);
	}
}

void AMainCharacter::MoveCameraAwayFromCharacter(float TransitionSmoothSpeed, float DeltaTime)
{
	if (FMath::Abs(this->CameraBoom->TargetArmLength - this->CameraBoomLengthCache) > 1.0f)
	{
		// The camera must get further away behind the character
		this->CameraBoom->TargetArmLength =
			FMath::Lerp(this->CameraBoom->TargetArmLength, this->CameraBoomLengthCache, TransitionSmoothSpeed * DeltaTime);
	}

	if (FMath::Abs(this->CameraBoomExtension->TargetArmLength - this->CameraBoomExtensionLengthCache) > 1.0)
	{
		// The camera must get closer to the right shoulder of the character
		this->CameraBoomExtension->TargetArmLength =
			FMath::Lerp(this->CameraBoomExtension->TargetArmLength, this->CameraBoomExtensionLengthCache, TransitionSmoothSpeed * DeltaTime);
	}
}

