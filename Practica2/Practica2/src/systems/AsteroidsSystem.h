#pragma once
#include "../ecs/System.h"
class AsteroidsSystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::_SYS_ASTEROIDS)

	AsteroidsSystem();
	virtual ~AsteroidsSystem();

	void recieve(const Message& m) override;
	void initSystem() override;
	void update() override;
private:
	void onCollission_AsteroidBullet(ecs::Entity* a);
	void onRoundOver();
	void onRoundStart();

	int numOfAsteroids_;
	bool active_;
};

