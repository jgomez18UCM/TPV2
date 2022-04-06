#pragma once
#include "../ecs/System.h"
class FighterSystem : public ecs::System
{
public:
	__SYSID_DECL__(ecs::_SYS_FIGHTER)

		FighterSystem() {};
	virtual ~FighterSystem() {};

	void recieve(const Message& m) override;
	void initSystem() override;
	void update() override;

private:
	void onCollission_FighterAsteroid();
	void onRoundOver();
	void onRoundStart();

	bool active_;
};

