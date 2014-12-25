// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "CharacterBase.h"


ACharacterBase::ACharacterBase(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	this->GetCapsuleComponent()->InitCapsuleSize(42.0f, 95.0f);

	// Don't rotate the character when the controller is rotated (This will affect only the camera)
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
	this->bUseControllerRotationYaw = false;

	// Configure default character state and movement
	this->bIsAiming = false;
	this->bIsFiring = false;
	this->bIsSprinting = false;
	this->bIsReloading = false;
	this->bIsDead = false;
	this->SprintSpeed = 700.0f; // in centimeters / second
	this->JogSpeed = 450.0f; // in centimeters / second
	this->AimSpeed = 200.0f; // in centimeters / second
	this->GetCharacterMovement()->MaxWalkSpeed = this->JogSpeed;
	this->GetCharacterMovement()->bOrientRotationToMovement = true;
	this->GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // The character rotates at this rotation rate
	this->GetCharacterMovement()->JumpZVelocity = 350.0f;
	this->GetCharacterMovement()->AirControl = 0.2f; // 20% of the total control

	// Set up the gameplay features
	this->HealthCapacity = 1000.0f;
	this->ShotsPerSecond = 10; // How many projectiles the character can fire each second
	this->GunMuzzleSocketName = "Muzzle"; // Default muzzle socket name
	this->GunMuzzleOffset = FVector(75.0f, 0.0f, 75.0f); // Default offset from the character location for projectiles to spawn

	// Set up the weapon
	this->Rifle = NewObject<UWeapon>(); // Create default rifle
	this->Rifle->WeaponType = EWeaponType::Rifle;
	this->EquippedWeapon = this->Rifle;

	// Note: The skeletal mesh and animation blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named BP_MainCharacter (to avoid direct content references in C++)
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	this->Health = this->HealthCapacity;

	if (this->GetMesh()->GetNumChildrenComponents() > 0)
	{
		this->GunMesh = Cast<USkeletalMeshComponent>(this->GetMesh()->GetChildComponent(0));
	}

	this->GetCharacterMovement()->MaxWalkSpeed = this->JogSpeed;

	this->FireDelay = 1.0f / this->ShotsPerSecond;
	this->FireDelayCounter = 0.0f;

	// Set up the weapon
	if (this->RifleClass != NULL)
	{
		UWeapon* RifleWeapon = Cast<UWeapon>(this->RifleClass->GetDefaultObject());
		if (RifleWeapon != NULL)
		{
			this->Rifle = NewObject<UWeapon>();
			this->Rifle->Init(*RifleWeapon);

			this->EquippedWeapon = this->Rifle;
		}
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bIsDead)
	{
		return;
	}

	if (this->EquippedWeapon != NULL && this->bIsFiring &&
		this->EquippedWeapon->WeaponType == EWeaponType::Rifle)
	{
		this->FireDelayCounter += DeltaTime;
		if (this->FireDelayCounter >= this->FireDelay)
		{
			this->OnFire();
			this->FireDelayCounter = 0.0f;
		}
	}
}

void ACharacterBase::MoveForward(float AxisValue)
{
	if (this->bIsDead)
	{
		return;
	}

	if (this->Controller != NULL && AxisValue != 0.0f)
	{
		FRotator ControllerRotation = this->GetControlRotation();
		FRotator YawRotation(0.0f, ControllerRotation.Yaw, 0.0f); // We need only the Yaw Rotation of the controller

		// Find the forward vector relative to the controller's yaw rotation
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		this->AddMovementInput(Direction, AxisValue);
	}
}

void ACharacterBase::MoveRight(float AxisValue)
{
	if (this->bIsDead)
	{
		return;
	}

	if (this->Controller != NULL && AxisValue != 0.0f)
	{
		FRotator ControllerRotation = this->GetControlRotation();
		FRotator YawRotation(0.0f, ControllerRotation.Yaw, 0.0f); // We need only the Yaw Rotation of the controller

		// Find the right vector relative to the controller's yaw rotation
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		this->AddMovementInput(Direction, AxisValue);
	}
}

void ACharacterBase::SprintStart()
{
	if (this->bIsDead)
	{
		return;
	}

	this->bIsSprinting = true;

	if (!this->bIsAiming && !this->bIsReloading)
	{
		this->GetCharacterMovement()->MaxWalkSpeed = this->SprintSpeed;
	}
}

void ACharacterBase::SprintStop()
{
	if (this->bIsDead)
	{
		return;
	}

	this->bIsSprinting = false;

	if (!this->bIsAiming)
	{
		this->GetCharacterMovement()->MaxWalkSpeed = this->JogSpeed;
	}
}

void ACharacterBase::AimStart()
{
	if (this->bIsDead)
	{
		return;
	}

	this->bIsAiming = true;

	this->bUseControllerRotationYaw = true; // While the character is aiming he must use the controller's yaw rotation
	this->GetCharacterMovement()->bOrientRotationToMovement = false; // And must not orient his rotation according to movement
	this->GetCharacterMovement()->MaxWalkSpeed = this->AimSpeed; // Has to use the AimSpeed
}

void ACharacterBase::AimStop()
{
	if (this->bIsDead)
	{
		return;
	}

	this->bIsFiring = false;
	this->bIsAiming = false;

	this->bUseControllerRotationYaw = false; // While the character is not aiming he must not use the controller's yaw rotation
	this->GetCharacterMovement()->bOrientRotationToMovement = true; // And must orient his rotation according to movement

	// The character needs to start walking normaly again 
	if (this->bIsSprinting)
	{
		this->GetCharacterMovement()->MaxWalkSpeed = this->SprintSpeed;
	}
	else
	{
		this->GetCharacterMovement()->MaxWalkSpeed = this->JogSpeed;
	}
}

void ACharacterBase::FireStart()
{
	if (this->bIsDead)
	{
		return;
	}

	if (!this->bIsReloading && this->bIsAiming)
	{
		this->bIsFiring = true;
		this->OnFire();
		this->FireDelayCounter = 0.0f;
	}
}

void ACharacterBase::FireStop()
{
	if (this->bIsDead)
	{
		return;
	}

	this->bIsFiring = false;
}

void ACharacterBase::OnFire()
{
	// Try and fire a projectile
	if (this->EquippedWeapon != NULL &&
		this->EquippedWeapon->ProjectileClass != NULL &&
		this->bIsAiming && !this->bIsReloading)
	{
		this->OnFireEvent();

		if (this->EquippedWeapon->AmmoInClip > 0)
		{
			// Find the spawn rotation of the projectile
			const FRotator SpawnRotation = this->GetControlRotation();

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

			UWorld* const World = this->GetWorld();
			if (World != NULL)
			{
				// Spawn the projectile at the muzzle
				World->SpawnActor<AProjectileBase>(this->EquippedWeapon->ProjectileClass, SpawnLocation, SpawnRotation);
				this->EquippedWeapon->AmmoInClip--;
			}
		}
	}
}

void ACharacterBase::ReloadStart()
{
	if (this->bIsDead)
	{
		return;
	}

	if ((this->EquippedWeapon != NULL) &&
		(this->EquippedWeapon->AmmoInClip < this->EquippedWeapon->ClipCapacity) &&
		(this->EquippedWeapon->RemainingAmmo > 0))
	{
		this->bIsFiring = false;
		this->bIsReloading = true;
		this->OnReloadEvent();

		if (!this->bIsAiming)
		{
			this->GetCharacterMovement()->MaxWalkSpeed = this->JogSpeed;
		}
	}
}

void ACharacterBase::ReloadStop()
{
	if (this->bIsDead)
	{
		return;
	}

	this->bIsReloading = false;

	// The character needs to start walking normaly again 
	if (this->bIsAiming)
	{
		this->GetCharacterMovement()->MaxWalkSpeed = this->AimSpeed;
	}
	else if (this->bIsSprinting)
	{
		this->GetCharacterMovement()->MaxWalkSpeed = this->SprintSpeed;
	}
	else
	{
		this->GetCharacterMovement()->MaxWalkSpeed = this->JogSpeed;
	}
}

void ACharacterBase::Reload()
{
	if (this->EquippedWeapon != NULL)
	{
		this->EquippedWeapon->Reload();
		this->ReloadStop();
	}
}

int32 ACharacterBase::PickUpRifleAmmo(int32 Ammo)
{
	int32 PickedUpAmount = 0;
	if (this->Rifle != NULL)
	{
		PickedUpAmount = this->Rifle->AddAmmo(Ammo);
	}

	return PickedUpAmount;
}

float ACharacterBase::GainHealth(float Health)
{
	float GainedHealthAmount = Health;

	this->Health += Health;
	if (this->Health > this->HealthCapacity)
	{
		GainedHealthAmount = Health - (this->Health - this->HealthCapacity);
		this->Health = this->HealthCapacity;
	}

	return GainedHealthAmount;
}

void ACharacterBase::TakeDamage(float Damage, const FHitResult& Hit, const AActor* DamageCauser)
{
	this->Health -= Damage;

	if (this->Health <= 0.0f)
	{
		this->bIsDead = true;

		this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

