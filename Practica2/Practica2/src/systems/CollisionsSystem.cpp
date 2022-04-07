// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../ecs/messages.h"
#include "../ecs/Manager.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "GameCtrlSystem.h"

CollisionsSystem::CollisionsSystem() :
		ballTr_(nullptr), active_(false) {
}

CollisionsSystem::~CollisionsSystem() {
}

void CollisionsSystem::initSystem() {
	
}

void CollisionsSystem::update() {

	if (!active_)
		return;

	bool playerHit = false;
	auto playerTr = mngr_->getComponent<Transform>(mngr_->getHandler(ecs::_hdlr_FIGHTER));
	for (auto a : mngr_->getEntities(ecs::_grp_ASTEROIDS)) {
		auto aTr = mngr_->getComponent<Transform>(a);
		if (Collisions::collidesWithRotation(aTr->pos_, aTr->width_, aTr->height_, aTr->rot_,
			playerTr->pos_, playerTr->width_, playerTr->height_, playerTr->rot_)) {
			playerHit = true;
			break;
		}
		for (auto b : mngr_->getEntities(ecs::_grp_BULLETS)) {
			auto bTr = mngr_->getComponent<Transform>(b);
			if (Collisions::collidesWithRotation(aTr->pos_, aTr->width_, aTr->height_, aTr->rot_,
				bTr->pos_, bTr->width_, bTr->height_, bTr->rot_)) {
				Message m;
				m.id = _m_COLLISSION_ASTEROID_BULLET;
				m.collission_asteroid_bullet.a = a;
				m.collission_asteroid_bullet.b = b;
				mngr_->send(m);
				break;
			}
		}
	}

	if (playerHit) {
		Message m;
		m.id = _m_COLLISSION_ASTEROID_FIGHTER;
		mngr_->send(m);
	}
}

void CollisionsSystem::recieve(const Message &m) {
	switch (m.id) {
	case _m_ROUND_START:
		active_ = true;
		break;
	case _m_ROUND_OVER:
		active_ = false;
		break;
	default:
		break;
	}
}
