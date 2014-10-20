
#include "TestingGround.h"
#include "BallProjectile.h"


ABallProjectile::ABallProjectile(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	this->Damage = 50.0f;
}
