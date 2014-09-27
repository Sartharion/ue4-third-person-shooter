

#include "TestingGround.h"
#include "BallProjectile.h"


ABallProjectile::ABallProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Use a sphere as a simple collision representation
	this->CollisionComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComponent"));
	this->CollisionComponent->InitSphereRadius(5.0f);
	//this->CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");			// Collision profiles are defined in DefaultEngine.ini
	this->CollisionComponent->OnComponentHit.AddDynamic(this, &ABallProjectile::OnHit);		// set up a notification for when this component overlaps something
	this->RootComponent = this->CollisionComponent;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	this->ProjectileMovement = PCIP.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComponent"));
	this->ProjectileMovement->UpdatedComponent = this->CollisionComponent;
	this->ProjectileMovement->InitialSpeed = 3000.f;
	this->ProjectileMovement->MaxSpeed = 3000.f;
	this->ProjectileMovement->bRotationFollowsVelocity = true;
	this->ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ABallProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(this->GetVelocity() * 100.0f, this->GetActorLocation());

		this->Destroy();
	}
}
