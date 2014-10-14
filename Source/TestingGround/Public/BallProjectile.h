

#pragma once

#include "GameFramework/Actor.h"
#include "BallProjectile.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API ABallProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubobjectPtr<class USphereComponent> CollisionComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TSubobjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	/** Called when projectile hits something */
	UFUNCTION()
	void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
