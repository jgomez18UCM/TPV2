#include "DeAcceleration.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

DeAcceleration::DeAcceleration() : tr_(nullptr), deAcc_(0.995f)
{
}

DeAcceleration::DeAcceleration(float deAcc) : tr_(nullptr), deAcc_(deAcc)
{
}

DeAcceleration::~DeAcceleration()	
{
}

void DeAcceleration::initComponent()
{
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void DeAcceleration::update()
{
	auto& vel = tr_->getVel();

	if (vel.magnitude() > 0.05f) {
		float speed = vel.magnitude() * deAcc_;
		vel = Vector2D(0,-speed).rotate(tr_->getRot());
	}
	if (vel.magnitude() <= 0.05f) {
		vel= Vector2D(0,0).rotate(tr_->getRot());
	}
}
