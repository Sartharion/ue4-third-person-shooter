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
	USphereComponent* CollisionComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	/** Gets the location where the projectile spawned */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	FVector GetSpawnLocation() const;

	/** Gets the damage that the projectile deals on impact */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	float GetDamage() const;

	/** Called when the projectile hits something */
	UFUNCTION(BlueprintImplementableEvent, Category = "Projectile")
	void OnImpact(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void BeginPlay() override;
	
protected:
	/** Called when projectile hits something */
	UFUNCTION()
	virtual void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	/** The location where the projectile spawned */
	FVector SpawnLocation;
};
