// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API AProjectileBase : public AActor
{
	GENERATED_UCLASS_BODY()

	/** The damage that the projectile deals on impact */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Damage;

	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubobjectPtr<class USphereComponent> CollisionComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	TSubobjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	/** Gets the damage that the projectile deals on impact */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual float GetDamage() const;

	/** Called when the projectile hits something */
	UFUNCTION(BlueprintImplementableEvent, Category = "Projectile")
	void OnImpact(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	/** Called when projectile hits something */
	UFUNCTION()
	virtual void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);	
};
