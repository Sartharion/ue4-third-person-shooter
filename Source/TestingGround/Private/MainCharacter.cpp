

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
	this->SprintSpeed = 700.0f; // in centimeters / second
	this->JogSpeed = 450.0f; // in centimeters / second
	this->AimSpeed = 200.0f; // in centimeters / second
	this->CharacterMovement->MaxWalkSpeed = this->JogSpeed;
	this->CharacterMovement->bOrientRotationToMovement = true;
	this->CharacterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // The character rotates at this rotation rate
	this->CharacterMovement->JumpZVelocity = 350.0f;
	this->CharacterMovement->AirControl = 0.2f; // 20% of the total control

	// Create a camera boom (pulls in towards the player if there is a collision)
	this->CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	this->CameraBoom->AttachTo(this->RootComponent);
	this->CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character in centimeters
	this->CameraBoom->bUseControllerViewRotation = true; // Rotate the arm based on the controller

	// Create an extension for the CameraBoom. This extension is used of OTS (Over The Shoulder) view
	this->CameraBoomExtension = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoomExtension"));
	this->CameraBoomExtension->AttachTo(this->CameraBoom, USpringArmComponent::SocketName);
	this->CameraBoomExtension->TargetArmLength = 30.0f; // The camera is this distance right of the target
	this->CameraBoomExtension->bUseControllerViewRotation = false; // Already uses controller view rotation, because is attached to the CameraBoom
	this->CameraBoomExtension->SetRelativeRotation(FRotator(-10.0f, -90.0f, 0.0f)); // Rotate it -90degrees so that the camera is over the right shoulder of the character

	// Create a follow camera
	this->FollowCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	this->FollowCamera->AttachTo(this->CameraBoomExtension, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	this->FollowCamera->bUseControllerViewRotation = false; // Already uses controller view rotation, because is attached to the CameraBoom
	this->FollowCamera->SetWorldRotation(FRotator::ZeroRotator);

	// Set up the gameplay parameters
	this->GunOffset = FVector(50.0f, 30.0f, 50.0f); // Default offset from the character location for projectiles to spawn
	this->CameraBoomLengthWhileAiming = 100.0f; // The camera is this distance behind the target
	this->CameraBoomExtensionLengthWhileAiming = 50.0f; // The camera is distance right of the target
	this->CameraTransitionSmoothSpeed = 15.0f; // The smooth speed at which the camera transitions between 2 points in space (A multipllier for DeltaTime)

	// Note: The skeletal mesh and animation blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named BP_MainCharacter (to avoid direct content references in C++)
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->CharacterMovement->MaxWalkSpeed = this->JogSpeed;
	this->CameraBoomLengthCache = this->CameraBoom->TargetArmLength;
	this->CameraBoomExtensionLengthCache = this->CameraBoomExtension->TargetArmLength;
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bIsAiming)
	{
		this->MoveCameraCloserToCharacter(this->CameraTransitionSmoothSpeed, DeltaTime);
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

	if (!this->bIsAiming)
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
	this->bIsFiring = true;
	this->OnFire();
}

void AMainCharacter::FireStop()
{
	this->bIsFiring = false;
}

void AMainCharacter::OnFire()
{
	// Try and fire a projectile
	if (this->ProjectileClass != NULL && this->bIsAiming)
	{
		const FRotator SpawnRotation = this->GetControlRotation();

		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = this->GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = this->GetWorld();
		if (World != NULL)
		{
			// Spawn the projectile at the muzzle
			World->SpawnActor<ABallProjectile>(this->ProjectileClass, SpawnLocation, SpawnRotation);
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
		InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
		InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

		InputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::SprintStart);
		InputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::SprintStop);
		InputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::AimStart);
		InputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::AimStop);
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::FireStart);
		InputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::FireStop);
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

