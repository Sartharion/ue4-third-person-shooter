// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "ProjectileBase.h"
#include "../Public/CharacterBase.h"


AProjectileBase::AProjectileBase(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Use a sphere as a simple collision representation
	this->CollisionComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, FName(TEXT("SphereComponent")));
	this->CollisionComponent->InitSphereRadius(5.0f);
	//this->CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile"); // Collision profiles are defined in DefaultEngine.ini
	this->CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit); // Set up a notification for when this component overlaps something
	this->RootComponent = this->CollisionComponent;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	this->ProjectileMovement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, FName(TEXT("ProjectileComponent")));
	this->ProjectileMovement->UpdatedComponent = this->CollisionComponent;
	this->ProjectileMovement->InitialSpeed = 3000.f;
	this->ProjectileMovement->MaxSpeed = 3000.f;
	this->ProjectileMovement->bRotationFollowsVelocity = true;
	this->ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	this->InitialLifeSpan = 3.0f;
	this->Damage = 0.0f;
}

float AProjectileBase::GetDamage() const
{
	return this->Damage;
}

void AProjectileBase::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this))
	{
		if ((OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(this->GetVelocity() * 100.0f, this->GetActorLocation());

			this->Destroy();
		}

		ACharacterBase* Character = Cast<ACharacterBase>(OtherActor);
		if (Character != NULL)
		{
			Character->TakeDamage(this->GetDamage());

			this->Destroy();
		}
	}
}

