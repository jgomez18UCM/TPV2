#include "Follow.h"
#include "Transform.h"
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
	tr_ = ent_->getComponent<Transform>();
	player_ = mngr_->getHandler(ecs::_hdlr_FIGHTER)->getComponent<Transform>();
	assert(tr_ != nullptr && player_ != nullptr);
}

void Follow::update()
{
	auto& mVel = tr_->getVel();
	auto playerPos = player_->getPos();
	auto mPos = tr_->getPos();

	mVel = mVel.rotate(mVel.angle(playerPos - mPos) > 0 ? 1.0f : -1.0f);
}
