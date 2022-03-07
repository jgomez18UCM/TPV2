#include "DisableOnExit.h"
#include "../ecs/Entity.h"
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
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void DisableOnExit::update()
{
	auto& pos = tr_->getPos();
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();

	// check left/right borders
	if (pos.getX() < -width) {
		ent_->setAlive(false);
	}
	else if (pos.getX() > sdlutils().width()) {
		ent_->setAlive(false);
	}

	// check upper/lower borders
	if (pos.getY() < -height) {
		ent_->setAlive(false);
	}
	else if (pos.getY() > sdlutils().height()) {
		ent_->setAlive(false);
	}
}
