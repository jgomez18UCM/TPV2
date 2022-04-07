#include "FighterSystem.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"
#include "../components/Image.h"
#include "../components/Health.h"
#include "../components/FighterCtrl.h"
#include "../components/ShowAtOppositeSide.h"
#include "../components/DeAcceleration.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

FighterSystem::FighterSystem() : player_(nullptr),  lastShot_(0), shotCd_(250), active_(false)
{
}

FighterSystem::~FighterSystem()
{
}

void FighterSystem::recieve(const Message& m)
{
	switch (m.id) {
		case _m_COLLISSION_ASTEROID_FIGHTER:
			onCollission_FighterAsteroid();
			break;
		case _m_ROUND_OVER:
			onRoundOver();
			break;
		case _m_ROUND_START:
			onRoundStart();
			break;
		case _m_NEW_GAME:
			onNewgame();
			break;
		default:
			break;
	}
}

void FighterSystem::initSystem()
{
    player_ = mngr_->addEntity();
	mngr_->setHandler(ecs::_hdlr_FIGHTER, player_);
	auto playerTr = mngr_->addComponent<Transform>(player_);
	auto s = 50.0f;
	auto x = (sdlutils().width() - s) / 2.0f + 100;
	auto y = (sdlutils().height() - s) / 2.0f;
	playerTr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
	mngr_->addComponent<Image>(player_, &sdlutils().images().at("fighter"));
	mngr_->addComponent<FighterCtrl>(player_)->setThrust(0.2f);
	mngr_->addComponent<Health>(player_, 3, &sdlutils().images().at("heart"));
	mngr_->addComponent<ShowAtoppositeSide>(player_);
	mngr_->addComponent<DeAcceleration>(player_);
}

void FighterSystem::update()
{
	if (!active_) return;
	auto playerTr = mngr_->getComponent<Transform>(player_);
	auto playerCtrl = mngr_->getComponent<FighterCtrl>(player_);
	auto& vel = playerTr->vel_;
	auto& rot = playerTr->rot_;
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		if (ihdlr.isKeyDown(playerCtrl->up_)) {
			auto& s = sdlutils().soundEffects().at("thrust");
			s.play();

			Vector2D newVel = vel + Vector2D(0, -1).rotate(rot) * mngr_->getComponent<FighterCtrl>(player_)->thrust_;
			// add 1.0f to the speed (respecting the limit 3.0f). Recall
			// that speed is the length of the velocity vector
			float speed = std::min(3.0f, newVel.magnitude());

			// change the length of velocity vecto to 'speed'. We need
			// '.rotate(rot)' for the case in which the current speed is
			// 0, so we rotate it to the same direction where the PacMan
			// is looking
			//
			vel = Vector2D(0, -speed).rotate(rot);
		}
		if (ihdlr.isKeyDown(playerCtrl->left_)) {
			rot-=5.0f;
		}
		if (ihdlr.isKeyDown(playerCtrl->right_)) {
			rot+=5.0f;
		}
		if (ihdlr.isKeyDown(playerCtrl->shoot_)) {
			if (lastShot_ + shotCd_ <= sdlutils().currRealTime()) {
				Message m;
				auto w = playerTr->width_;
				auto h = playerTr->height_;
				auto bulletPos = playerTr->pos_ + Vector2D(w / 2.0f, h / 2.0f) - Vector2D(0.0f, h / 2.0f + 5.0f + 12.0f).rotate(rot) - Vector2D(2.0f, 10.0f);
				auto bulletVel = Vector2D(0.0f, -1.0f).rotate(rot) * (vel.magnitude() + 5.0f);
				m.id = _m_SHOOT;
				m.shoot.posX = bulletPos.getX();
				m.shoot.posY = bulletPos.getY();
				m.shoot.velX = bulletVel.getX();
				m.shoot.velY = bulletVel.getY();
				m.shoot.width = 5.0f;
				m.shoot.height = 20.0f;
				mngr_->send(m);
				lastShot_ = sdlutils().currRealTime();
				sdlutils().soundEffects().at("gunshot").play();
			}
		}
	}

	vel = vel * mngr_->getComponent<DeAcceleration>(player_)->deAcc_;
	mngr_->getComponent<Transform>(player_)->move();
	mngr_->getComponent<ShowAtoppositeSide>(player_)->check();

}

void FighterSystem::onCollission_FighterAsteroid()
{	
	auto Tr = mngr_->getComponent<Transform>(player_);
	Tr->pos_.set((sdlutils().width() - Tr->width_) / 2.0f + 100, (sdlutils().height() - Tr->height_) / 2.0f);
	Tr->vel_.set(0, 0);
}

void FighterSystem::onRoundOver()
{
	active_ = false;
}

void FighterSystem::onRoundStart()
{
	active_ = true;
}

void FighterSystem::onNewgame()
{
	mngr_->getComponent<Health>(mngr_->getHandler(ecs::_hdlr_FIGHTER))->resetHealth();
}
