

#include "TestingGround.h"
#include "MainCharacter.h"


AMainCharacter::AMainCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->CapsuleComponent->InitCapsuleSize(45.0f, 95.0f);

	// Don't rotate the character when the controller is rotated (This will affect only the camera)
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
	this->bUseControllerRotationYaw = false;

	// Configure default character state and movement
	this->bIsAiming = false;
	this->bIsFiring = false;
	this->bIsSprinting = false;
	this->bIsReloading = false;
	this->SprintSpeed = 700.0f; // in centimeters / second
	this->JogSpeed = 450.0f; // in centimeters / second
	this->AimSpeed = 200.0f; // in centimeters / second
	this->CharacterMovement->MaxWalkSpeed = this->JogSpeed;
	this->CharacterMovement->bOrientRotationToMovement = true;
	this->CharacterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // The character rotates at this rotation rate
	this->CharacterMovement->JumpZVelocity = 350.0f;
	this->CharacterMovement->AirControl = 0.2f; // 20% of the total control

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

	// Set up the gameplay parameters
	this->GunOffset = FVector(150.0f, 0.0f, 75.0f); // Default offset from the character location for projectiles to spawn
	this->ShotsPerSecond = 10; // How many projectiles the character can fire each second
	this->CameraBoomLengthWhileAiming = 100.0f; // The camera is this distance behind the target
	this->CameraBoomExtensionLengthWhileAiming = 50.0f; // The camera is distance right of the target
	this->CameraTransitionSmoothSpeed = 15.0f; // The smooth speed at which the camera transitions between 2 points in space (A multipllier for DeltaTime)
	this->AmmoCapacity = 300;
	this->ClipCapacity = 30;
	this->HealthCapacity = 1000.0f;

	// Note: The skeletal mesh and animation blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named BP_MainCharacter (to avoid direct content references in C++)
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->CharacterMovement->MaxWalkSpeed = this->JogSpeed;
	this->CameraBoomLengthCache = this->CameraBoom->TargetArmLength;
	this->CameraBoomExtensionLengthCache = this->CameraBoomExtension->TargetArmLength;

	this->FireDelay = 1.0f / this->ShotsPerSecond;
	this->FireDelayCounter = 0.0f;

	this->Ammo = this->AmmoCapacity - this->ClipCapacity;
	this->AmmoInClip = this->ClipCapacity;
	this->Health = this->HealthCapacity;
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bIsAiming)
	{
		this->MoveCameraCloserToCharacter(this->CameraTransitionSmoothSpeed, DeltaTime);

		if (this->bIsFiring)
		{
			this->FireDelayCounter += DeltaTime;
			if (this->FireDelayCounter >= this->FireDelay)
			{
				this->OnFire();
				this->FireDelayCounter = 0.0f;
			}
		}
	}
	else
	{
		this->MoveCameraAwayFromCharacter(this->CameraTransitionSmoothSpeed, DeltaTime);
	}
}

void AMainCharacter::MoveForward(float AxisValue)
{
	if (this->Controller != NULL && AxisValue != 0.0f)
	{
		FRotator ControllerRotation = this->FollowCamera->GetComponentRotation();
		FRotator YawRotation = FRotator(0.0f, ControllerRotation.Yaw, 0.0f); // We need only the Yaw Rotation of the controller

		// Find the forward vector relative to the controller's yaw rotation
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		this->AddMovementInput(Direction, AxisValue);
	}
}

void AMainCharacter::MoveRight(float AxisValue)
{
	if (this->Controller != NULL && AxisValue != 0.0f)
	{
		FRotator ControllerRotation = this->FollowCamera->GetComponentRotation();
		FRotator YawRotation = FRotator(0.0f, ControllerRotation.Yaw, 0.0f); // We need only the Yaw Rotation of the controller

		// Find the right vector relative to the controller's yaw rotation
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		this->AddMovementInput(Direction, AxisValue);
	}
}

void AMainCharacter::SprintStart()
{
	this->bIsSprinting = true;

	if (!this->bIsAiming && !this->bIsReloading)
	{
		this->CharacterMovement->MaxWalkSpeed = this->SprintSpeed;
	}
}

void AMainCharacter::SprintStop()
{
	this->bIsSprinting = false;

	if (!this->bIsAiming)
	{
		this->CharacterMovement->MaxWalkSpeed = this->JogSpeed;
	}
}

void AMainCharacter::AimStart()
{
	this->bIsAiming = true;
	
	this->bUseControllerRotationYaw = true; // While the character is aiming he must use the controller's yaw rotation
	this->CharacterMovement->bOrientRotationToMovement = false; // And must not orient his rotation according to movement
	this->CharacterMovement->MaxWalkSpeed = this->AimSpeed; // Has to use the AimSpeed
}

void AMainCharacter::AimStop()
{
	this->bIsFiring = false;
	this->bIsAiming = false;
	
	this->bUseControllerRotationYaw = false; // While the character is not aiming he must not use the controller's yaw rotation
	this->CharacterMovement->bOrientRotationToMovement = true; // And must orient his rotation according to movement

	// The character needs to start walking normaly again 
	if (this->bIsSprinting)
	{
		this->CharacterMovement->MaxWalkSpeed = this->SprintSpeed;
	}
	else
	{
		this->CharacterMovement->MaxWalkSpeed = this->JogSpeed;
	}
}

void AMainCharacter::FireStart()
{
	if (!this->bIsReloading && this->bIsAiming)
	{
		this->bIsFiring = true;
		this->OnFire();
		this->FireDelayCounter = 0.0f;
	}
}

void AMainCharacter::FireStop()
{
	this->bIsFiring = false;
}

void AMainCharacter::OnFire()
{
	// Try and fire a projectile
	if (this->ProjectileClass != NULL &&
		this->bIsAiming &&
		!this->bIsReloading &&
		this->AmmoInClip > 0)
	{
		const FRotator SpawnRotation = this->GetControlRotation();

		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = this->GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = this->GetWorld();
		if (World != NULL)
		{
			// Spawn the projectile at the muzzle
			World->SpawnActor<ABallProjectile>(this->ProjectileClass, SpawnLocation, SpawnRotation);
			this->AmmoInClip--;
		}
	}
}

void AMainCharacter::ReloadStart()
{
	if (this->AmmoInClip < this->ClipCapacity &&
		this->Ammo > 0)
	{
		this->bIsFiring = false;
		this->bIsReloading = true;

		if (!this->bIsAiming)
		{
			this->CharacterMovement->MaxWalkSpeed = this->JogSpeed;
		}
	}
}

void AMainCharacter::ReloadStop()
{
	this->bIsReloading = false;

	// The character needs to start walking normaly again 
	if (this->bIsAiming)
	{
		this->CharacterMovement->MaxWalkSpeed = this->AimSpeed;
	}
	else if (this->bIsSprinting)
	{
		this->CharacterMovement->MaxWalkSpeed = this->SprintSpeed;
	}
	else
	{
		this->CharacterMovement->MaxWalkSpeed = this->JogSpeed;
	}
}

void AMainCharacter::Reload()
{
	if (this->AmmoInClip < this->ClipCapacity &&
		this->Ammo > 0)
	{
		int32 AmmoToReload = this->ClipCapacity - this->AmmoInClip;

		if (this->Ammo > AmmoToReload)
		{
			this->AmmoInClip += AmmoToReload;
			this->Ammo -= AmmoToReload;
		}
		else
		{
			this->AmmoInClip += this->Ammo;
			this->Ammo = 0;
		}
	}

	this->ReloadStop();
}

int32 AMainCharacter::PickUpAmmo(int32 Ammo)
{
	int32 PickedUpAmount = Ammo;

	this->Ammo += Ammo;
	if (this->Ammo > this->AmmoCapacity)
	{
		PickedUpAmount = Ammo - (this->Ammo - this->AmmoCapacity);
		this->Ammo = this->AmmoCapacity;
	}

	return PickedUpAmount;
}

void AMainCharacter::TakeDamage(float Damage)
{
	this->Health -= Damage;
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	if (InputComponent != NULL)
	{
		InputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
		InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
		InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

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

