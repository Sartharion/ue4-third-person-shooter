// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGround.h"
#include "ProjectileBase.h"
#include "DamageableObject.h"


AProjectileBase::AProjectileBase(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	// Use a sphere as a simple collision representation
	this->CollisionComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, FName(TEXT("CollisionComponent")));
	this->CollisionComponent->InitSphereRadius(2.5f);
	this->CollisionComponent->bCanEverAffectNavigation = false;
	this->CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit); // Set up a notification for when this component overlaps something
	this->RootComponent = this->CollisionComponent;
	//this->CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile"); // Collision profiles are defined in DefaultEngine.ini

	// Use a ProjectileMovementComponent to govern this projectile's movement
	this->ProjectileMovement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, FName(TEXT("ProjectileComponent")));
	this->ProjectileMovement->UpdatedComponent = this->CollisionComponent;
	this->ProjectileMovement->InitialSpeed = 7000.0f;
	this->ProjectileMovement->MaxSpeed = 7000.0f;
	this->ProjectileMovement->ProjectileGravityScale = 0.0f;
	this->ProjectileMovement->bRotationFollowsVelocity = true;
	this->ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	this->InitialLifeSpan = 3.0f;
	this->Damage = 0.0f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	this->SpawnLocation = this->GetActorLocation();
}

FVector AProjectileBase::GetSpawnLocation() const
{
	return this->SpawnLocation;
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
		}

		IDamageableObject* DamageableObject = Cast<IDamageableObject>(OtherActor);
		if (DamageableObject != NULL)
		{
			DamageableObject->TakeDamage(this->GetDamage(), Hit, this);
		}
	}

	this->OnImpact(OtherActor, OtherComp, NormalImpulse, Hit);
	this->Destroy();
}

