#include "DeAcceleration.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
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
	tr_ = mngr_->getComponent<Transform>(ent_);
	assert(tr_ != nullptr);
}

void DeAcceleration::update()
{
	auto& vel = tr_->vel_;

	if (vel.magnitude() > 0.05f) {
		float speed = vel.magnitude() * deAcc_;
		vel = Vector2D(0,-speed).rotate(tr_->rot_);
	}
	if (vel.magnitude() <= 0.01f) {
		vel= Vector2D(0,0).rotate(tr_->rot_);
	}
}
