#include "DisableOnExit.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"

DisableOnExit::DisableOnExit() : tr_()
{
}

DisableOnExit::~DisableOnExit()
{
}

void DisableOnExit::initComponent()
{
	tr_ = mngr_->getComponent<Transform>(ent_);
	assert(tr_ != nullptr);
}

void DisableOnExit::update()
{
	auto& pos = tr_->pos_;
	auto width = tr_->width_;
	auto height = tr_->height_;

	// check left/right borders
	if (pos.getX() < -width) {
		mngr_->setAlive(ent_, false);
	}
	else if (pos.getX() > sdlutils().width()) {
		mngr_->setAlive(ent_, false);
	}

	// check upper/lower borders
	if (pos.getY() < -height) {
		mngr_->setAlive(ent_, false);
	}
	else if (pos.getY() > sdlutils().height()) {
		mngr_->setAlive(ent_, false);
	}
}
