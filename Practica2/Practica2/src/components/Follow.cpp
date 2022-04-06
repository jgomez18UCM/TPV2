#include "Follow.h"
#include "Transform.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

Follow::Follow() : tr_(), player_()
{
}

Follow::~Follow()
{
}

void Follow::initComponent()
{
	tr_ = mngr_->getComponent<Transform>(ent_);
	player_ = mngr_->getComponent<Transform>(mngr_->getHandler(ecs::_hdlr_FIGHTER));
	assert(tr_ != nullptr && player_ != nullptr);
}

void Follow::follow()
{
	auto& mVel = tr_->vel_;
	auto playerPos = player_->pos_;
	auto mPos = tr_->pos_;

	mVel = mVel.rotate(mVel.angle(playerPos - mPos) > 0 ? 1.0f : -1.0f);
}
