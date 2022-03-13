// This file is part of the course TPV2@UCM - Samir Genaim

#include "FighterCtrl.h"

#include <cassert>

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "Image.h"

#include "Transform.h"

FighterCtrl::FighterCtrl() :
		tr_(nullptr), thrust_(0.2f), canMove_(false) {
}

FighterCtrl::~FighterCtrl() {
}

void FighterCtrl::initComponent() {
	tr_ = ent_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void FighterCtrl::update() {

	auto &ihldr = ih();
	if (!canMove_) return;
	if (ihldr.keyDownEvent()) {

		auto &vel_ = tr_->getVel();
		auto rot = tr_->getRot();

		if (ihldr.isKeyDown(SDL_SCANCODE_RIGHT)) { // rotate right
			tr_->setRot(rot + 5.0f);

			// also rotate the PacMan so it looks in the same
			// direction where it moves
			//
			vel_ = vel_.rotate(5.0f);
		} else if (ihldr.isKeyDown(SDL_SCANCODE_LEFT)) { // rotate left
			tr_->setRot(rot - 5.0f);

			// also rotate the PacMan so it looks in the same
			// direction where it moves
			//
			vel_ = vel_.rotate(-5.0f);
		}
		if (ihldr.isKeyDown(SDL_SCANCODE_UP)) { // increase speed
			auto& s = sdlutils().soundEffects().at("thrust");
			s.play();
			std::cout << "acelerando" << std::endl;
			
			Vector2D newVel = vel_ + Vector2D(0, -1).rotate(rot) * thrust_;
			// add 1.0f to the speed (respecting the limit 3.0f). Recall
			// that speed is the length of the velocity vector
			float speed = std::min(3.0f, newVel.magnitude());
			
			// change the length of velocity vecto to 'speed'. We need
			// '.rotate(rot)' for the case in which the current speed is
			// 0, so we rotate it to the same direction where the PacMan
			// is looking
			//
			vel_ = Vector2D(0, -speed).rotate(rot);			
		} 
	}
}
