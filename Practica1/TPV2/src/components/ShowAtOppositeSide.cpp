// This file is part of the course TPV2@UCM - Samir Genaim

#include "ShowAtOppositeSide.h"

#include <cassert>

#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

ShowAtoppositeSide::ShowAtoppositeSide() :
		tr_() {
}

ShowAtoppositeSide::~ShowAtoppositeSide() {
}

void ShowAtoppositeSide::initComponent() {
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void ShowAtoppositeSide::update() {
	auto &pos = tr_->getPos();
	auto &vel = tr_->getVel();

	// check left/right borders
	if (pos.getX() < 0) {
		pos.setX(sdlutils().width() - tr_->getWidth());
		//vel.set(0.0f, 0.0f);
	} else if (pos.getX() + tr_->getWidth() > sdlutils().width()) {
		pos.setX(0);
		//vel.set(0.0f, 0.0f);
	}

	// check upper/lower borders
	if (pos.getY() < 0) {
		pos.setY(sdlutils().height() - tr_->getHeight());
		//vel.set(0.0f, 0.0f);
	} else if (pos.getY() + tr_->getHeight() > sdlutils().height()) {
		pos.setY(0);
		//vel.set(0.0f, 0.0f);
	}
}
