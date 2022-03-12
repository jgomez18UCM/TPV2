#pragma once
#include "../ecs/Component.h"

class AsteroidsManager : public ecs::Component
{
public:
	__CMPID_DECL__(ecs::_ASTEROIDSMANAGER)
	AsteroidsManager();
	AsteroidsManager(int asteroidLimit, int delay);
	~AsteroidsManager();
	void update() override;
	void onCollision(ecs::Entity* e);
	void startRound();
	int getAsteroids() { return asteroids_; };
protected:
	void generateNewAsteroid(bool typeB);
	void crashAsteroid(ecs::Entity* e);
	int asteroidLimit_;
	int asteroids_;
	int spawnDelay_;
	int lastSpawn_;
};

