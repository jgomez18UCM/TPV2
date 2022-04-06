#pragma once
#include "../ecs/System.h"
#include "../utils/Vector2D.h"
class BulletsSystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::_SYS_BULLETS)

		BulletsSystem() {};
	virtual ~BulletsSystem() {};

	void recieve(const Message& m) override;
	void initSystem() override;
	void update() override;
private:
	void shoot(Vector2D pos, Vector2D vel, double width, double height);
	void onCollision_BulletAsteroid(ecs::Entity* a);
	void onRoundOver();
	void onRoundStart();

	bool active_;
};

