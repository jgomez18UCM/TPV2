// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Entity.h"
#include "../ecs/System.h"

struct Transform;

class PacManSystem: public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_PACMAN)

	PacManSystem();
	virtual ~PacManSystem();
	void initSystem() override;
	void update() override;
private:
	Transform *pmTR_;
};

