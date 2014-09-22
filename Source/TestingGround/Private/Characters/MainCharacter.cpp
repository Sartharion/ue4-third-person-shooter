

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

	// Configure default character movement
	this->bIsSprinting = false;
	this->SprintSpeed = 700.0f; // in centimeters / second
	this->JogSpeed = 450.0f; // in centimeters / second
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
	this->CameraBoomExtension->TargetArmLength = 25.0f; // The length of the extension
	this->CameraBoomExtension->bUseControllerViewRotation = false; // Already uses controller view rotation, because is attached to the CameraBoom
	this->CameraBoomExtension->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f)); // Rotate it -90degrees so that the camera is over the right shoulder of the character

	// Create a follow camera
	this->FollowCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	this->FollowCamera->AttachTo(this->CameraBoomExtension, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	this->FollowCamera->bUseControllerViewRotation = false; // Already uses controller view rotation, because is attached to the CameraBoom
	this->FollowCamera->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	// Note: The skeletal mesh and animation blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named BP_MainCharacter (to avoid direct content references in C++)
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->CharacterMovement->MaxWalkSpeed = this->JogSpeed;
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
	this->CharacterMovement->MaxWalkSpeed = this->SprintSpeed;
}

void AMainCharacter::SprintStop()
{
	this->CharacterMovement->MaxWalkSpeed = this->JogSpeed;
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
	}
}

