#pragma once
#include "../ecs/Component.h"
class Entity;
class AsteroidsManager : public ecs::Component
{
public:
	AsteroidsManager();
	AsteroidsManager(int asteroidLimit, int delay);
	~AsteroidsManager();
	void update() override;
	void onCollision(Entity* e);
	void startRound();
protected:
	void generateAsteroid(bool typeB);
	int asteroidLimit_;
	int asteroids_;
	int spawnDelay_;
	int lastSpawn_;
};

