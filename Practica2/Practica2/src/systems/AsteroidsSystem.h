#pragma once
#include "../ecs/System.h"
class AsteroidsSystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::_SYS_ASTEROIDS)

	AsteroidsSystem();
	AsteroidsSystem(int limit);
	virtual ~AsteroidsSystem() {};

	void recieve(const Message& m) override;
	void initSystem() override;
	void update() override;
private:
	void onCollission_AsteroidBullet(ecs::Entity* a);
	void onRoundOver();
	void onRoundStart();
	void generateAsteroidsStart(int n);
	void generateNewAsteroid(bool typeB);
	void divideAsteroid(ecs::Entity* a);

	int numOfAsteroids_;
	int limitAsteroids_;
	bool active_;
};

