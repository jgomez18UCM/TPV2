// This file is part of the course TPV2@UCM - Samir Genaim

#include "ShowAtOppositeSide.h"

#include <cassert>

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

ShowAtoppositeSide::ShowAtoppositeSide() :
		tr_() {
}

ShowAtoppositeSide::~ShowAtoppositeSide() {
}

void ShowAtoppositeSide::initComponent() {
	tr_ = mngr_->getComponent<Transform>(ent_);
	assert(tr_ != nullptr);
}

void ShowAtoppositeSide::update() {
	auto &pos = tr_->pos_;
	auto &vel = tr_->vel_;
	auto width = tr_->width_;
	auto height = tr_->height_;

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
