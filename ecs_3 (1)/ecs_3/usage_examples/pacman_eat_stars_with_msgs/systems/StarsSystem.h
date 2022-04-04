// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

using ecs::Entity;

class StarsSystem: public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_STARS)

	StarsSystem();
	virtual ~StarsSystem();
	void initSystem() override;
	void update() override;
	void addStar(unsigned int n);
	void onStarEaten(Entity *e);
	void recieve(const Message &m) override;
private:
	unsigned int starsLimit_;
	unsigned int currNumOfStars_;
};

