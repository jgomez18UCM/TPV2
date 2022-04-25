// This file is part of the course TPV2@UCM - Samir Genaim

#include "BallSystem.h"

#include "../components/Image.h"
#include "../ecs/Manager.h"
#include "../components/Transform.h"
#include "../sdlutils/SDLUtils.h"
#include "NetworkSystem.h"

BallSystem::BallSystem() :
		ballTr_(nullptr) {
}

BallSystem::~BallSystem() {
}

void BallSystem::initSystem() {
	// create the ball entity
	//
	auto ball = mngr_->addEntity();
	mngr_->setHandler(ecs::_hdlr_BALL, ball);

	ballTr_ = mngr_->addComponent<Transform>(ball);
	auto ballSize = 15.0f;
	auto ballX = (sdlutils().width() - ballSize) / 2.0f;
	auto ballY = (sdlutils().height() - ballSize) / 2.0f;
	ballTr_->init(Vector2D(ballX, ballY), Vector2D(), ballSize, ballSize, 0.0f);

	mngr_->addComponent<Image>(ball, &sdlutils().images().at("tennis_ball"));
}

void BallSystem::update() {

	ballTr_->move();

	// check upper/lower borders
	if (ballTr_->pos_.getY() < 0) {
		ballTr_->pos_.setY(0.0f);
		ballTr_->vel_.setY(-ballTr_->vel_.getY());
		// play some sound
		sdlutils().soundEffects().at("wall_hit").play();
	} else if (ballTr_->pos_.getY() + ballTr_->width_ > sdlutils().height()) {
		ballTr_->pos_.setY(sdlutils().height() - ballTr_->height_);
		ballTr_->vel_.setY(-ballTr_->vel_.getY());
		sdlutils().soundEffects().at("wall_hit").play();
	}

//	mngr_->getSystem<NetworkSystem>()->sendBallPosition(ballTr_);
}

void BallSystem::resetBall() {
	ballTr_->pos_.set(sdlutils().width() / 2 - 5, sdlutils().height() / 2 - 5);
	ballTr_->vel_.set(0, 0);
	mngr_->getSystem<NetworkSystem>()->sendBallPosition(ballTr_);
	mngr_->getSystem<NetworkSystem>()->sendBallVelocity(ballTr_);
}

void BallSystem::changeBallPos(float x, float y) {
	ballTr_->pos_.set(x, y);
}

void BallSystem::changeBallVel(float x, float y) {
	ballTr_->vel_.set(x, y);
}

void BallSystem::moveBall() {
	auto &r = sdlutils().rand();

	int dx = 1 - 2 * r.nextInt(0, 2); // 1 or -1
	int dy = 1 - 2 * r.nextInt(0, 2); // 1 or -1

	Vector2D v(dx * r.nextInt(2, 7), dy * r.nextInt(2, 7));
	ballTr_->vel_.set(v.normalize() * 2);
	mngr_->getSystem<NetworkSystem>()->sendBallVelocity(ballTr_);
}

void BallSystem::recieve(const Message &m) {
	if (!mngr_->getSystem<NetworkSystem>()->isHost())
		return;

	switch (m.id) {
	case _m_ROUND_START:
		moveBall();
		break;
	case _m_NEW_GAME:
	case _m_ROUND_OVER:
		resetBall();
		break;
	default:
		break;
	}
}
