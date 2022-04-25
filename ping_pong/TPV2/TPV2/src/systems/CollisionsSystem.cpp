// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../ecs/messages.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "GameCtrlSystem.h"
#include "NetworkSystem.h"
CollisionsSystem::CollisionsSystem() :
		ballTr_(nullptr), active_(false) {
}

CollisionsSystem::~CollisionsSystem() {
}

void CollisionsSystem::initSystem() {
	ballTr_ = mngr_->getComponent<Transform>(
			mngr_->getHandler(ecs::_hdlr_BALL));
	assert(ballTr_ != nullptr);
}

void CollisionsSystem::update() {

	if (!mngr_->getSystem<NetworkSystem>()->isHost())
		return;

	if (!active_)
		return;

	bool ballCollidesWithPaddle = false;

	for (auto e : mngr_->getEntities(ecs::_grp_PADDLES)) {
		auto paddleTr_ = mngr_->getComponent<Transform>(e);
		ballCollidesWithPaddle = Collisions::collides(paddleTr_->pos_,
				paddleTr_->width_, paddleTr_->height_, ballTr_->pos_,
				ballTr_->width_, ballTr_->height_);

		if (ballCollidesWithPaddle)
			break;
	}

	if (ballCollidesWithPaddle) {

		// change the direction of the ball, and increment the speed
		auto &vel = ballTr_->vel_; // the use of & is important, so the changes goes directly to the ball
		vel.setX(-vel.getX());
		vel = vel * 1.2f;

		mngr_->getSystem<NetworkSystem>()->sendBallVelocity(ballTr_);
		// play some sound
		sdlutils().soundEffects().at("paddle_hit").play();
	} else if (ballTr_->pos_.getX() < 0) {
		Message m;
		m.id = _m_BALL_EXIT;
		m.ball_exit.side = GameCtrlSystem::LEFT;
		mngr_->send(m);
	} else if (ballTr_->pos_.getX() + ballTr_->width_ > sdlutils().width()) {
		Message m;
		m.id = _m_BALL_EXIT;
		m.ball_exit.side = GameCtrlSystem::RIGHT;
		mngr_->send(m);
	}
}

void CollisionsSystem::recieve(const Message &m) {
	switch (m.id) {
	case _m_ROUND_START:
		active_ = true;
		break;
	case _m_NEW_GAME:
	case _m_ROUND_OVER:
		active_ = false;
		break;
	default:
		break;
	}
}
