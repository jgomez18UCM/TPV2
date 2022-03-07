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
	auto width = tr_->getWidth();
	auto height = tr_->getHeight();

	// check left/right borders
	if (pos.getX() < -width) {
		pos.setX(sdlutils().width());
	} else if (pos.getX() > sdlutils().width()) {
		pos.setX(-width);
	}

	// check upper/lower borders
	if (pos.getY() < -height) {
		pos.setY(sdlutils().height());
	} else if (pos.getY()> sdlutils().height()) {
		pos.setY(-height);
	}
}
