#pragma once
#include "../ecs/Component.h"
class Entity;
class AsteroidsManager : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ASTEROIDSMANAGER)
	AsteroidsManager();
	AsteroidsManager(int asteroidLimit, int delay);
	~AsteroidsManager();
	void update() override;
	void onCollision(Entity* e);
	void startRound();
protected:
	void generateNewAsteroid(bool typeB);
	void crashAsteroid();
	int asteroidLimit_;
	int asteroids_;
	int spawnDelay_;
	int lastSpawn_;
};

