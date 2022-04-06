#include "BulletsSystem.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../components/Transform.h"

BulletsSystem::BulletsSystem() : active_(false)
{
}

BulletsSystem::~BulletsSystem()
{
}

void BulletsSystem::recieve(const Message& m)
{
	switch(m.id) {
		case _m_ROUND_OVER:
			onRoundOver();
			break;
		case _m_ROUND_START:
			onRoundStart();
			break;
		case _m_COLLISSION_ASTEROID_BULLET:
			onCollision_BulletAsteroid(m.collission_asteroid_bullet.b);
			break;
		case _m_SHOOT:
			shoot(Vector2D(m.shoot.posX, m.shoot.posY), Vector2D(m.shoot.velX, m.shoot.posX), m.shoot.width, m.shoot.height);
			break;
		default:
			break;
	}
}

void BulletsSystem::initSystem()
{
}

void BulletsSystem::update()
{
	if (active_) {
		for (ecs::Entity* b : mngr_->getEntities(ecs::_grp_BULLETS)) {
			mngr_->getComponent<Transform>(b)->move();
		}
	}
}

void BulletsSystem::shoot(Vector2D pos, Vector2D vel, double width, double height)
{
	auto bullet = mngr_->addEntity(ecs::_grp_BULLETS);
	auto bulletTr = mngr_->addComponent<Transform>(bullet);
	bulletTr->init(pos, vel, width, height, vel.angle(Vector2D(0, -1)));
}

void BulletsSystem::onCollision_BulletAsteroid(ecs::Entity* b)
{
	mngr_->setAlive(b, false);
}

void BulletsSystem::onRoundOver()
{
	for (ecs::Entity* b : mngr_->getEntities(ecs::_grp_BULLETS)) {
		mngr_->setAlive(b, false);
	}
	active_ = false;
}

void BulletsSystem::onRoundStart()
{
	active_ = true;
}
